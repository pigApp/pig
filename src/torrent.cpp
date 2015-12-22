#include "torrent.h"
#include "threadedsocket.h"

#include <stdlib.h>

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/extensions/ut_pex.hpp>// Por defecto enable.

#include <QDir>
#include <QTimer>
#include <QDebug>//

const int KB = 1024;

Torrent::Torrent(const QString* const PIG_PATH, const QString *host, const QString *url,
                 const QString *pkg, int scene, Player **player, QObject *parent) :
    QObject(parent),
    _PIG_PATH(PIG_PATH),
    _host(host),
    _url(url),
    _pkg(pkg),
    _player(player),
    _scene(scene)
{
    isDump = true;
    hasMetadata = false;
    isSkip = false;
    isAborted = false;
    kb_required = 20480; //5120;
    kb_skip_global = 0;


    ThreadedSocket *thread = new ThreadedSocket(_PIG_PATH, _host, _url, _pkg);

    connect (thread, SIGNAL(sendFile(int, QString)), this, SLOT(init(int, QString)));
    connect (thread, SIGNAL(sendError(QString)), this, SLOT(error(QString)));
    connect (thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

Torrent::~Torrent()
{
    isAborted = true;
//    if (mSocket != NULL)
//        delete mSocket;
    if (h.is_valid())
        s->remove_torrent(h);
}

void Torrent::init(int ID, QString path)
{
    Q_UNUSED(ID);

    qDebug() << path;
    //delete mSocket;
    //mSocket = NULL;

    libtorrent::session_settings ss;
    libtorrent::add_torrent_params p;
    libtorrent::error_code ec;

    s = new libtorrent::session();
    ss.disable_hash_checks = true;
    s->set_settings(ss);
    s->set_alert_mask(2147483647); //(1864);
    s->start_dht();

    QString pt = (*_PIG_PATH);
    pt.append("/tmp/torrents/movies/");
    p.save_path = pt.toStdString();
    p.ti = new libtorrent::torrent_info("/home/lxfb/.pig/tmp/torrents/test.torrent", ec);//
    //p.ti = new libtorrent::torrent_info(file->at(0).toStdString(), ec);

    h = s->add_torrent(p, ec);
    h.set_sequential_download(true);
    h.set_priority(255);

    if (!ec)
        main_loop();
    else
        error("ERROR");
}

void Torrent::main_loop()
{
    qDebug() << "MAIN_LOOP";

    if (!isAborted) {
        std::auto_ptr<libtorrent::alert> a = s->pop_alert();

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
                std::cout << "//// CHACHE_FLUSHED" << std::endl;
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

        //(*_player)->debug = QString::fromStdString(a->message());

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

    /*
    fs = h.torrent_file().get()->orig_files();

    for (int i=0; i<fs.num_files(); i++) {
        if (check) {
            for (int n=0; n<formats.size(); n++) {
                if (QString::fromStdString(fs.file_name(i)).endsWith(formats[n]
                    , Qt::CaseInsensitive)) {
                    if (_scene == ctrl) {
                        priorities.push_back(7);
                        _scene = i;
                        check = false;
                    } else {
                        priorities.push_back(0);
                        ++ctrl;
                    }
                    break;
                } else {
                    if (n == formats.size()+1)
                        priorities.push_back(0);
                }
            }
        } else {
            priorities.push_back(0);
        }
    }
    */

    h.prioritize_files(priorities); //TODO: PROBAR ESTO SOLO.

    _scene = 0;//

    fs = h.torrent_file().get()->files();
    piece_first = fs.map_file(_scene, 0, 0).piece;
    n_kb = fs.file_size(_scene)/KB;

    //(*_player)->kb_required = kb_required;
    //(*_player)->n_kb = n_kb;
    //(*_player)->status = "";

    hasMetadata = true;

    qDebug() << "PIECE_FIRST: " << piece_first;
    qDebug() << "N_KB: " << n_kb;
}

void Torrent::stats()
{
    if (!isAborted) {
        h.flush_cache();

        const qint64 kb_writen = (s->get_cache_status().blocks_written)*16; // ((s->get_cache_status().blocks_written)*16)/KB

        (*_player)->kb_writen = QString::number(kb_writen);
        //(*_player)->bitrate = QString::number(h.status(2).download_rate/KB);
        //(*_player)->kb_writen = QString::number(h.status(2).num_peers);

        if (isDump) {
            if ((kb_writen-kb_skip_global) >= kb_required) {
                isDump = false;
                h.flush_cache(); //TODO: Recibirlo con un Alert.
                //PLAYER PLAY
            }
        }

        /*
        if (player != NULL) {
            int total_sec = 6658730/1000;
            int total_mb = 836600/1024;
            int current_sec = libvlc_media_player_get_time(player->mediaplayer)/1000;
            qint64 current_mb_to_sec = ((total_sec*(kb_writen/1024))/total_mb);

            if (current_sec >= 10) {
                if ((current_sec >= current_mb_to_sec) && libvlc_media_player_is_playing(player->mediaplayer)) {
                    libvlc_media_player_pause(player->mediaplayer);
                    qDebug() << "-- PAUSED";
                } else if ((current_sec <= current_mb_to_sec) && !libvlc_media_player_is_playing(player->mediaplayer)) {
                    libvlc_media_player_play(player->mediaplayer);
                    qDebug() << "-- PLAY";
                }
            }

            qDebug() << "SEC " << current_sec << "|" << "MB_TO_SEC" << current_mb_to_sec;
        }
        */
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
    std::cout << "VALID: " << h.is_valid() << std::endl;

    if (!isAborted) { //TODO: Hacerlo con alert.
        if ((h.status(32).state != 0) && (h.status(32).state != 1) && (h.status(32).state != 2))
            filter_files();
        else
            QTimer::singleShot(1000, this, SLOT(get()));
    }
}

bool Torrent::piece_available(qint64 total_msec, qint64 offset_msec)
{
    //SIZE
    //qDebug() << "FILE_SIZE_TORRENT: " << h.get_torrent_info().total_size();

    //piece_offset = piece_offset_global+((offset_msec*n_pieces)/total_msec);
    //return h.have_piece(piece_offset);
}

void Torrent::piece_update(qint64 total_msec, qint64 offset_msec)
{

    std::vector<int> piecePriority;
    isSkip = true;

    //piece_offset = 475; //piece_offset_global+(((offset_msec)*n_pieces)/total_msec);
    piece_offset = piece_offset_global+((99*n_pieces)/100);
    offset = (piece_offset_global+((offset_msec*n_pieces)/total_msec))*(piece_length/1024);
    kb_skip_global = h.status().total_done/MB;

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


    h.prioritize_pieces(piecePriority);
    isDump();

}
*/

/*
VER PRIORIDADES.
for (std::size_t i=0; i<priorities.size(); ++i)
    std::cout << priorities[i] << ' ';
std::cout << std::endl;
for(std::size_t i=0; i<h.file_priorities().size(); ++i)
    std::cout << h.file_priorities()[i] << ' ' << std::endl;
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
            const qint64 kb_downloaded = offset+(h.status().total_wanted_done/1024);
            //(*_player)->information(total, kb_downloaded, 220);
        } else {
            const int downloadedSkip_mb = (h.status().total_done/MB)-kb_skip_global;
            //(*_player)->information(0, 0, downloadedSkip_mb);
        }
        QTimer::singleShot(1000, this, SLOT(main_loop()));
    }
}
*/
