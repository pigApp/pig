#include "torrent.h"

#include <stdlib.h>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/extensions/ut_pex.hpp>

#include <QDir>
#include <QTimer>
#include <QDebug>

const int KB = 1024;
const int MB = 1048576;

Torrent::Torrent(QObject *parent, QObject **root
    , const QString *url, const int _scene)
    : QObject(parent) //TODO: Cambiar el nombre _scene.
{
    _root = root;
    scene = _scene;
    dump = true;
    metadata_ready = false;
    skip = false;
    abort = false;
    mb_required = 4; //10;
    mb_skip_global = 0;
#ifdef __linux__
    const QString home = QDir::homePath();
    const std::string tmp = home.toStdString()+"/.pig/tmp/";
#else
    const std::string tmp = "C:/PIG/.pig/tmp/";
#endif

    libtorrent::session_settings ss;
    libtorrent::add_torrent_params p;
    libtorrent::error_code ec;
    s = new libtorrent::session();
    ss.disable_hash_checks = true; //Posible causa del error al filtrar las filas.
    s->set_settings(ss);
    s->set_alert_mask(2147483647);// limitar.
    //s->set_alert_mask(864);//
    s->listen_on(std::make_pair(6881, 6889), ec);
    if (ec) qDebug() << "ERROR_PORT"; // TODO: Crear funcion de error.
    s->add_extension(&libtorrent::create_ut_pex_plugin);
    s->start_upnp();
    s->start_dht();
    p.url = url->toStdString();
    p.save_path = tmp;
    h = s->add_torrent(p, ec);
    h.set_sequential_download(true);
    h.set_priority(255);

    if (ec)
        qDebug() << "ERROR_ADD_TORRENT"; // TODO: Crear funcion de error.
    else
        main_loop();
}

Torrent::~Torrent()
{
    abort = true;
    s->remove_torrent(h);
    (*_root)->disconnect(this);
}

void Torrent::main_loop()
{
    //if (h.is_valid()) Posiblemente sea necesario.
    //if (!abort) Posiblemente sea necesario.

    std::auto_ptr<libtorrent::alert> a = s->pop_alert();
    switch (a->type())
    {
        case libtorrent::torrent_added_alert::alert_type:
        {
            metadata_ready = true;
            filter_files();
            (*_root)->setProperty("status", "");
            break;
        }
        case libtorrent::cache_flushed_alert::alert_type: // No lo toma.
        {
            std::cout << "//////// CHACHE_FLUSHED" << std::endl;
            break;
        }
        default: break;
    }
    if (metadata_ready)
        progress();

    (*_root)->setProperty("debug", QString::fromStdString(a->message()));

    //std::cout << "TYPE : " << a->type() << std::endl;//
    //std::cout << "MSG : " << a->message() << std::endl;//

    QTimer::singleShot(1000, this, SLOT(main_loop()));
}

void Torrent::filter_files()
{
    bool check = true;
    int ctrl = 1;
    QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264"
        << ".mkv" << ".mp4" << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
    std::vector<int> priorities;
    fs = h.torrent_file().get()->orig_files();

    for (int i=0; i<fs.num_files(); i++) {
        if (check) {
            for (int n=0; n<formats.size(); n++) {
                if (QString::fromStdString(fs.file_name(i)).endsWith(formats[n]
                    , Qt::CaseInsensitive)) {
                    if (scene == ctrl) {
                        priorities.push_back(7);
                        scene = i;
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
    h.prioritize_files(priorities);

    fs = h.torrent_file().get()->files();
    piece_first = fs.map_file(scene, 0, 0).piece;
    n_mb = fs.file_size(scene)/MB;

    (*_root)->setProperty("mb_required", mb_required);
    (*_root)->setProperty("n_mb", n_mb);
}

void Torrent::ret()
{
    if (!skip) {
        (*_root)->setProperty("movie_file_path"
            , QString::fromStdString(fs.file_path(scene, h.status(128).save_path)));
    } else {
        skip = false;
        //UPDATE PLAYER
    }
}

void Torrent::progress()
{
    if (!abort) {
        const int mb_downloaded = h.status(2).total_payload_download/MB;

        //qDebug() << "DOWN_ORG_: " << h.status(2).total_wanted_done/MB; //NO SUMA CHACHE.
        qDebug() << "DOWN: " << h.status(2).total_payload_download/MB; //SUMA CACHE.

        (*_root)->setProperty("bitRate", QString::number(h.status(2).download_rate/KB));
        (*_root)->setProperty("peers", h.status(2).num_peers);
        (*_root)->setProperty("mb_downloaded", mb_downloaded);
        if (dump) {
            if ((mb_downloaded-mb_skip_global) >= mb_required) {
                dump = false;
                h.flush_cache(); // TODO: Recibirlo con un Alert.
                ret();
                //QTimer::singleShot(3000, this, SLOT(ret()));//
            }
        }
    }
}











/*
void Torrent::get()
{
    std::cout << "VALID: " << h.is_valid() << std::endl;

    if (!abort) { //TODO: Hacerlo con alert.
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
    mb_skip_global = h.status().total_done/MB;

    qDebug() << "-- SCENE: " << scene;
    qDebug() << "-- TOTAL_MSEC: " << total_msec;
    qDebug() << "-- OFFSET_MESEC: " << offset_msec;
    qDebug() << "-- PIECE_FIRST: " << piece_first;
    qDebug() << "-- PIECE_LAST: " << piece_last;
    qDebug() << "-- PIECE_TOTAL: " << n_pieces;
    qDebug() << "-- PIECE_TOTAL_GLOBAL: " << n_pieces_global;
    qDebug() << "-- PIECES_OFFSET_GLOBAL: " << piece_offset_global;
    qDebug() << "-- TOTAL_PRE_SKIP: " << mb_skip_global;
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
        //mb_required = remnant-1; //TODO: Cuando este solucionado el tema del slider, revisar esto.
    //else
        //mb_required = 15;


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
    if (!abort) {
        if (!skip) {
            const qint64 kb_downloaded = offset+(h.status().total_wanted_done/1024);
            //(*_player)->progress(total, kb_downloaded, 220);
        } else {
            const int downloadedSkip_mb = (h.status().total_done/MB)-mb_skip_global;
            //(*_player)->progress(0, 0, downloadedSkip_mb);
        }
        QTimer::singleShot(1000, this, SLOT(main_loop()));
    }
}
*/
