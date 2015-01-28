#include "torrent.h"

#include <libtorrent/extensions/ut_pex.hpp>

#include <stdlib.h>

#include <QDir>
#include <QTimer>
#include <QDebug>

const int KB = 1024;
const int MB = 1048576;

Torrent::Torrent(QObject *parent, const QString *url) : QObject(parent)
{
    dump = true;
    skip = false;
    abort = false;
    mb_required = 10;
    mb_skip_global = 0;
    piece_offset_global = 0;

#ifdef __linux__
    const QString home = QDir::homePath();
    const std::string path = home.toStdString()+"/.pig/tmp/";
#else
    const std::string path = "C:/PIG/.pig/tmp/";
#endif

    //TODO: Compilar libtorrent sin deprecated.

    libtorrent::session_settings ss;
    ss.disable_hash_checks = true;
    libtorrent::add_torrent_params p;
    libtorrent::error_code ec; //TODO: Error code.

    s = new libtorrent::session;
    s->set_settings(ss);
    s->add_extension(&libtorrent::create_ut_pex_plugin);
    s->listen_on(std::make_pair(6881, 6889), ec);
    s->start_dht();
    s->start_upnp();
    p.save_path = path;
    p.url = url->toStdString();
    h = s->add_torrent(p);
    h.set_sequential_download(true);
    h.set_priority(255);

    get();
}

Torrent::~Torrent()
{
    abort = true;
    s->remove_torrent(h);
    (*_root)->disconnect(this);
}

void Torrent::get()
{
    if (!abort) { //TODO: Hacerlo con alert.
        if ((h.status(32).state != 0) && (h.status(32).state != 1) && (h.status(32).state != 2))
            filter();
        else
            QTimer::singleShot(1000, this, SLOT(get()));
    }
}

void Torrent::filter()
{
    boost::intrusive_ptr<const libtorrent::torrent_info> ti = h.torrent_file();
    libtorrent::file_storage fs = ti.get()->orig_files();

    QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264" << ".mkv" << ".mp4"
        << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
    std::vector<int> priorities;

    scene = scene-1; //
    file = QString::fromStdString(fs.file_name(0)); //

    //FIX: Filtrar filas por prioridad. Al aplicar las prioridades, el video baja corrupto.
    /*
    bool check = true;
    int ctrl = 1;
    for (int i=0; i<fs.num_files(); i++) {
        if (check) {
            file = QString::fromStdString(fs.file_name(i));
            for (int fts=0; fts<formats.size(); fts++) {
                if (file.endsWith(formats[fts], Qt::CaseInsensitive)) {
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
                    if (fts == formats.size()+1)
                        priorities.push_back(0);
                }
            }
        } else {
            priorities.push_back(0);
        }
    }
    h.prioritize_files(priorities);

    VER PRIORIDADES.
    for(std::size_t i=0; i<priorities.size(); ++i)
        std::cout << priorities[i] << ' ';
    std::cout << std::endl;

    for(std::size_t i=0; i<h.file_priorities().size(); ++i)
        std::cout << h.file_priorities()[i] << ' ';
    std::cout << std::endl;
    */

    fs = ti.get()->files();
    piece_first = fs.map_file(scene, 0, 0).piece;
    piece_last = fs.map_file(scene, fs.file_size(scene), 0).piece;
    piece_length = fs.piece_length();
    n_pieces = piece_last-piece_first;
    for (int i=0; i<(scene+1); i++ )
        piece_offset_global = piece_offset_global+(fs.map_file(i, fs.file_size(i), 0).piece-fs.map_file(i, 0, 0).piece);
    n_pieces_global = fs.num_pieces();
    n_mb = (n_pieces*piece_length)/KB;

    progress();
}

void Torrent::progress()
{
    if (!abort) {
        const int mb_downloaded = h.status(2).total_wanted_done/MB;
        (*_root)->setProperty("bitRate", QString::number(h.status(2).download_rate/KB));
        (*_root)->setProperty("peers", h.status(2).num_peers);
        (*_root)->setProperty("n_mb", n_mb);
        (*_root)->setProperty("mb_required", mb_required);
        (*_root)->setProperty("mb_downloaded", mb_downloaded);
        if (dump) {
            if ((mb_downloaded-mb_skip_global) >= mb_required) {
                if (!skip) {
                    dump = false;
                    QDir dir(QString::fromStdString(h.status(128).save_path)+QString::fromStdString(h.status(64).name));
                    if (!dir.exists())
                        dir.setPath(QString::fromStdString(h.status(128).save_path));
                    (*_root)->setProperty("videoPath", dir.absolutePath()+"/"+file);
                } else {
                    dump = false;
                    skip = false;
                    //UPDATE PLAYER
                }
            }
        }
        QTimer::singleShot(1000, this, SLOT(progress()));
    }
}

/*
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
void Torrent::progress()
{
    if (!abort) {
        if (!skip) {
            const qint64 kb_downloaded = offset+(h.status().total_wanted_done/1024);
            //(*_player)->progress(total, kb_downloaded, 220);
        } else {
            const int downloadedSkip_mb = (h.status().total_done/MB)-mb_skip_global;
            //(*_player)->progress(0, 0, downloadedSkip_mb);
        }
        QTimer::singleShot(1000, this, SLOT(progress()));
    }
}
*/
