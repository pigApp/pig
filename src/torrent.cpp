#include "torrent.h"

#include <stdlib.h>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
//#include <libtorrent/extensions/ut_pex.hpp>// Por defecto enable.

#include <QDir>
#include <QTimer>
#include <QDebug>//
#include <QMediaPlayer>//
#include <QVideoProbe>//
#include <QVideoWidget>//

const int KB = 1024;

Torrent::Torrent(QObject *parent, QObject **root, const int *const scene) : QObject(parent)
{
    mSocket = NULL;
    _root = root;
    _scene = (*scene);
    dump = true;
    metadata_ready = false;
    skip = false;
    aborted = false;
    kb_required = 5120;
    kb_skip_global = 0;
}

Torrent::~Torrent()
{
    aborted = true;
    if (mSocket != NULL)
        delete mSocket;
    if (h.is_valid())
        s->remove_torrent(h);
    (*_root)->disconnect(this);
}

void Torrent::get(const QString *const host, const QString *const url, const QString *const target)
{
    TcpSocket *mSocket = new TcpSocket();
    mSocket->host = (*host);
    mSocket->urls << (*url);
    mSocket->targets << (*target);
    mSocket->start();
    connect (mSocket, SIGNAL(sig_ret_files(const QString *const, const QStringList *const))
        , this, SLOT(start(const QString *const, const QStringList *const)));
    connect (mSocket, SIGNAL(sig_err()), this, SLOT(error()));
}

void Torrent::start(const QString *const tmp, const QStringList *const file)
{
    delete mSocket;
    mSocket = NULL;

    libtorrent::session_settings ss;
    libtorrent::add_torrent_params p;
    libtorrent::error_code ec;
    s = new libtorrent::session();
    ss.disable_hash_checks = true;
    s->set_settings(ss);
    s->set_alert_mask(2147483647); //(1864);
    s->start_dht();
    p.save_path = tmp->toStdString();
    p.ti = new libtorrent::torrent_info("/home/lxfb/.pig/tmp/test.torrent", ec);//
    //p.ti = new libtorrent::torrent_info(file->at(0).toStdString(), ec);
    h = s->add_torrent(p, ec);
    h.set_sequential_download(true);
    h.set_priority(255);

    if (!ec)
        main_loop();
    else
        error();
}

void Torrent::main_loop()
{
    if (!aborted) {
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
                error();
                break;
            }
            default: break;
        }
        (*_root)->setProperty("debug", QString::fromStdString(a->message()));

        if (metadata_ready) stats();

        QTimer::singleShot(1000, this, SLOT(main_loop()));
    }
}

void Torrent::filter_files()
{
    bool check = true;
    int ctrl = 1;
    QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264"
        << ".mkv" << ".mp4" << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
    std::vector<int> priorities;

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

    (*_root)->setProperty("kb_required", kb_required);
    (*_root)->setProperty("n_kb", n_kb);
    (*_root)->setProperty("status", "");

    metadata_ready = true;
}

void Torrent::ret()
{
    QMediaPlayer *player = new QMediaPlayer();
    QVideoProbe *probe = new QVideoProbe;
    probe->setSource(player);
    connect(probe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));

    QVideoWidget *videoWidget = new QVideoWidget;
    player->setVideoOutput(videoWidget);
    player->setMedia(QUrl::fromLocalFile(QString::fromStdString(fs.file_path(_scene, h.status(128).save_path))));
    player->play();
    videoWidget->show();

    //TODO: BORRAR multimediawidgets EN PIG.PRO, BORRAR #includes Y SLOT processFrame(QVideoFrame fr)

    /*
    if (!skip) {
        (*_root)->setProperty("movie_file_path"
            , QString::fromStdString(fs.file_path(_scene, h.status(128).save_path)));
    } else {
        skip = false;
        //UPDATE PLAYER
    }
    */
}

void Torrent::processFrame(QVideoFrame fr)
{
    qDebug() << fr.isValid();
}

void Torrent::stats()
{
    if (!aborted) {
        h.flush_cache();//
        const double kb_writen = (s->get_cache_status().blocks_written)*16;
        (*_root)->setProperty("kb_writen", kb_writen);
        (*_root)->setProperty("bitRate", QString::number(h.status(2).download_rate/KB));
        (*_root)->setProperty("peers", h.status(2).num_peers);
        if (dump) {
            if ((kb_writen-kb_skip_global) >= kb_required) {
                dump = false;
                //h.flush_cache(); // TODO: Recibirlo con un Alert.
                //ret();
                //QTimer::singleShot(1000, this, SLOT(ret()));//3000
            }
        } //else {
            //kb_writen = ((s->get_cache_status().blocks_written)*16)/KB;
            //(*_root)->setProperty("kb_writen", kb_writen);
        //}

        qDebug() << kb_writen;
    }
}

void Torrent::error()
{
    (*_root)->setProperty("status", "TORRENT ERROR");
}










/*
void Torrent::get()
{
    std::cout << "VALID: " << h.is_valid() << std::endl;

    if (!aborted) { //TODO: Hacerlo con alert.
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
    skip = true;

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
    dump();

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
    if (!aborted) {
        if (!skip) {
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
