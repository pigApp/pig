#include "torrent.h"
#include "threadedsocket.h"

#include <stdlib.h>

#include <QDir>
#include <QTimer>
#include <QDebug>//

const int KB = 1024;

Torrent::Torrent(const QString* const PIG_PATH, const QString *host, const QString *url,
                 const QString *pkg, int scene, Movie **movie, QObject *parent) :
    QObject(parent),
    _PIG_PATH(PIG_PATH),
    _host(host),
    _url(url),
    _pkg(pkg),
    _scene(scene),
    _player(movie),
    piece_first(0),
    kb_required(20000), //5120
    kb_skip_global(0),
    n_kb(0),
    hasMetadata(false),
    isDump(true),
    isSkip(false),
    isAborted(false)
{
    ThreadedSocket *thread = new ThreadedSocket(_PIG_PATH, _host, _url, _pkg);

    connect (thread, SIGNAL(sendFile(int, QString)), this, SLOT(init(int, QString)));
    connect (thread, SIGNAL(sendError(QString)), this, SLOT(error(QString)));
    connect (thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

Torrent::~Torrent()
{
    isAborted = true;
    
    if (handle.is_valid())
        session.remove_torrent(handle);
}

void Torrent::init(int ID, QString path)
{
    Q_UNUSED(ID);

    libtorrent::session_settings ss;
    ss.disable_hash_checks = true;
    
    libtorrent::add_torrent_params p;
    libtorrent::error_code ec;

    session.set_settings(ss);
    session.set_alert_mask(2147483647); //(1864);
    session.listen_on(std::make_pair(6881, 6889), ec);
    session.start_dht();

    p.save_path = (*_PIG_PATH).toStdString()+"/tmp/torrents/movies/";
    p.ti = new libtorrent::torrent_info("/home/lxfb/.pig/tmp/torrents/FOXX.torrent", ec);
    //p.ti = new libtorrent::torrent_info(path.toStdString(), ec); //(path, ec);
    //p.ti = new libtorrent::torrent_info(file->at(0).toStdString(), ec);

    handle = session.add_torrent(p, ec);
    handle.set_sequential_download(true);
    handle.set_priority(255);

    if (!ec)
        main_loop();
    else
        error("ERROR");
}

void Torrent::main_loop()
{
    //qDebug() << "MAIN_LOOP";

    if (!isAborted) {

        std::auto_ptr<libtorrent::alert> a = session.pop_alert();

        switch (a->type())
        {
            case libtorrent::add_torrent_alert::alert_type:
            {
                filter_files();
                break;
            }
            // HACER UN FLUSH DESDE ACA CADA VEZ QUE SE DESCARGUEN 2 PIEZAS COMPLETAS.
            case libtorrent::cache_flushed_alert::alert_type: // No lo toma.
            {
                //std::cout << "//// CHACHE_FLUSHED" << std::endl;
                break;
            }
            case libtorrent::file_error_alert::alert_type:
            case libtorrent::tracker_error_alert::alert_type:
            case libtorrent::torrent_error_alert::alert_type:
            {
                error("ERROR");
                break;
            }
            default: break;
        }

        if (hasMetadata) stats();

        QTimer::singleShot(1000, this, SLOT(main_loop()));
    }
}

void Torrent::filter_files()
{
    bool check = true;
    int ctrl = 1;
    QStringList formats;
    std::vector<int> priorities;

    formats << ".avi" << ".divx" << ".flv" << ".h264" << ".mkv" << ".mp4"
            << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";

//    storage = handle.torrent_file().get()->orig_files();

//    for (int i=0; i<storage.num_files(); i++) {
//        if (check) {
//            for (int n=0; n<formats.size(); n++) {
//                if (QString::fromStdString(storage.file_name(i)).endsWith(formats[n]
//                    , Qt::CaseInsensitive)) {
//                    if (_scene == ctrl) {
//                        priorities.push_back(7);
//                        _scene = i;
//                        check = false;
//                    } else {
//                        priorities.push_back(0);
//                        ++ctrl;
//                    }
//                    break;
//                } else {
//                    if (n == formats.size()+1)
//                        priorities.push_back(0);
//                }
//            }
//        } else {
//            priorities.push_back(0);
//        }
//    }
//

    //handle.prioritize_files(priorities); //TODO: PROBAR ESTO SOLO.

    _scene = 0;//

    storage = handle.torrent_file().get()->files();
    piece_first = storage.map_file(_scene, 0, 0).piece;
    n_kb = storage.file_size(_scene)/KB;

    hasMetadata = true;
}

void Torrent::stats()
{
    if (!isAborted) {
        handle.flush_cache();

        const qint64 kb_writen = (session.get_cache_status().blocks_written)*16; //((session->get_cache_status().blocks_written)*16)/KB

        emit sendStats((handle.status(2).download_rate/KB), handle.status(2).num_peers,
                       kb_writen, kb_required, n_kb);
        
        if (isDump) {
            if ((kb_writen-kb_skip_global) >= kb_required) {
                isDump = false;
                
                //handle.flush_cache(); //TODO: Recibirlo con un Alert.
               
                QString path_trucho;
                path_trucho = "The.Martian.2015.1080p.WEB-DL.DD5.1.H264-RARBG/The.Martian.2015.1080p.WEB-DL.DD5.1.H264-RARBG.mkv";

                emit sendFile(path_trucho);
                //emit sendFile(QString::fromStdString(storage.file_path(_scene)));
            }
        }

//      if (movie != NULL) {
//          int total_sec = 6658730/1000;
//          int total_mb = 836600/1024;
//          int current_sec = libvlc_media_player_get_time(player->mediaplayer)/1000;
//          qint64 current_mb_to_sec = ((total_sec*(kb_writen/1024))/total_mb);

//          if (current_sec >= 10) {
//              if ((current_sec >= current_mb_to_sec) && libvlc_media_player_is_playing(player->mediaplayer)) {
//                  libvlc_media_player_pause(player->mediaplayer);
//                  qDebug() << "-- PAUSED";
//              } else if ((current_sec <= current_mb_to_sec) && !libvlc_media_player_is_playing(player->mediaplayer)) {
//                  libvlc_media_player_play(player->mediaplayer);
//                  qDebug() << "-- PLAY";
//              }
//          }
//      }
    }
}

void Torrent::error(QString error)
{
    qDebug() << error;
//  (*_player)->status = "TORRENT ERROR";
}


























/*
void Torrent::get()
{
    std::cout << "VALID: " << handle.is_valid() << std::endl;

    if (!isAborted) { //TODO: Hacerlo con alert.
        if ((handle.status(32).state != 0) && (handle.status(32).state != 1) && (handle.status(32).state != 2))
            filter_files();
        else
            QTimer::singleShot(1000, this, SLOT(get()));
    }
}

bool Torrent::piece_available(qint64 total_msec, qint64 offset_msec)
{
    //SIZE
    //qDebug() << "FILE_SIZE_TORRENT: " << handle.get_torrent_info().total_size();

    //piece_offset = piece_offset_global+((offset_msec*n_pieces)/total_msec);
    //return handle.have_piece(piece_offset);
}

void Torrent::piece_update(qint64 total_msec, qint64 offset_msec)
{

    std::vector<int> piecePriority;
    isSkip = true;

    //piece_offset = 475; //piece_offset_global+(((offset_msec)*n_pieces)/total_msec);
    piece_offset = piece_offset_global+((99*n_pieces)/100);
    offset = (piece_offset_global+((offset_msec*n_pieces)/total_msec))*(piece_length/1024);
    kb_skip_global = handle.status().total_done/MB;

    qDebug() << "-- SCENE: " << scene;
    qDebug() << "-- TOTAL_MSEC: " << total_msec;
    qDebug() << "-- OFFSET_MESEC: " << offset_msec;
    qDebug() << "-- PIECE_FIRST: " << piece_first;
    qDebug() << "-- PIECE_LAST: " << piece_last;
    qDebug() << "-- PIECE_TOTAL: " << n_pieces;
    qDebug() << "-- PIECE_TOTAL_GLOBAL: " << n_pieces_global;
    qDebug() << "-- PIECES_OFFSET_GLOBAL: " << piece_offset_global;
    qDebug() << "-- TOTAL_PRE_SKIP: " << kb_skip_global;
    qDebug() << "-- OFFSET_PIECE_FILE: " << piece_offset;

    for (int i=0; i < (n_pieces_global+1); i++)
        if (i < piece_offset)
            piecePriority.push_back(0);
        else if (i >= piece_offset && i <= piece_last)
            piecePriority.push_back(7);
        else
            piecePriority.push_back(0);


    //const qint64 remnant = n_pieces_global-((offset_msec*n_pieces)/total_msec);
    //if (((remnant*piece_length)/MB) < 15)
        //kb_required = remnant-1; //TODO: Cuando este solucionado el tema del slider, revisar esto.
    //else
        //kb_required = 15;


    handle.prioritize_pieces(piecePriority);
    isDump();

}
*/

/*
VER PRIORIDADES.
for (std::size_t i=0; i<priorities.size(); ++i)
    std::cout << priorities[i] << ' ';
std::cout << std::endl;
for(std::size_t i=0; i<handle.file_priorities().size(); ++i)
    std::cout << handle.file_priorities()[i] << ' ' << std::endl;
*/

/*
for (int i=0; i < (n_pieces_global+1); i++) {
    if (i < piece_offset) {
        piecePriority.push_back(0);
    } else if (i >= piece_offset && i <= piece_last) {
        if (offsetPiecesLast_file != 0 && offsetPiecesLast_file > piece_offset) {
            if (i < offsetPiecesLast_file)
                piecePriority.push_back(7);
            else
                piecePriority.push_back(0);
        } else {
            piecePriority.push_back(7);
        }
    } else {
        piecePriority.push_back(0);
    }
}
offsetPiecesLast_file = piece_offset;
*/

/*
void Torrent::main_loop()
{
    if (!isAborted) {
        if (!isSkip) {
            const qint64 kb_downloaded = offset+(handle.status().total_wanted_done/1024);
            //(*_player)->information(total, kb_downloaded, 220);
        } else {
            const int downloadedSkip_mb = (handle.status().total_done/MB)-kb_skip_global;
            //(*_player)->information(0, 0, downloadedSkip_mb);
        }
        QTimer::singleShot(1000, this, SLOT(main_loop()));
    }
}
*/
