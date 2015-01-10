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
    aborted = false;
    skip = false;
    required = 10;
    skipGlobal = 0;
    pieceOffsetGlobal = 0;

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
    aborted = true;
    s->remove_torrent(h);
    (*_root)->disconnect(this);
}

void Torrent::get()
{
    if (!aborted) { //TODO: Hacerlo con alert.
        if (h.status(32).state != 0 && h.status(32).state != 1 && h.status(32).state != 2)
            filter();
        else
            QTimer::singleShot(1000, this, SLOT(get()));
    }
}

void Torrent::filter()
{
    boost::intrusive_ptr<const libtorrent::torrent_info> ti = h.torrent_file();
    libtorrent::file_storage fs = ti.get()->orig_files();

    QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264" << ".mkv" << ".mp4" << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
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
    pieceFirst = fs.map_file(scene, 0, 0).piece;
    pieceLast = fs.map_file(scene, fs.file_size(scene), 0).piece;
    pieceTotal = pieceLast-pieceFirst;
    pieceLength = fs.piece_length();
    pieceTotalGlobal = fs.num_pieces();
    total = (pieceTotal*pieceLength)/KB;
    for (int i=0; i<(scene+1); i++ )
        pieceOffsetGlobal = pieceOffsetGlobal+(fs.map_file(i, fs.file_size(i), 0).piece-fs.map_file(i, 0, 0).piece);
        
    progress();
}

void Torrent::progress()
{
    if (!aborted) {
        const int downloaded = h.status(2).total_wanted_done/MB;
        (*_root)->setProperty("bitRate", QString::number(h.status(2).download_rate/KB));
        (*_root)->setProperty("peers", h.status(2).num_peers);
        (*_root)->setProperty("total", total);
        (*_root)->setProperty("required", required);
        (*_root)->setProperty("downloaded", downloaded);
        if (dump) {
            if ((downloaded-skipGlobal) >= required) {
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

    //pieceOffset = pieceOffsetGlobal+((offset_msec*pieceTotal)/total_msec);
    //return h.have_piece(pieceOffset);
}

void Torrent::piece_update(qint64 total_msec, qint64 offset_msec)
{

    std::vector<int> piecePriority;
    skip = true;

    //pieceOffset = 475; //pieceOffsetGlobal+(((offset_msec)*pieceTotal)/total_msec);
    pieceOffset = pieceOffsetGlobal+((99*pieceTotal)/100);
    offset = (pieceOffsetGlobal+((offset_msec*pieceTotal)/total_msec))*(pieceLength/1024);
    skipGlobal = h.status().total_done/MB;

    qDebug() << "-- SCENE: " << scene;
    qDebug() << "-- TOTAL_MSEC: " << total_msec;
    qDebug() << "-- OFFSET_MESEC: " << offset_msec;
    qDebug() << "-- PIECE_FIRST: " << pieceFirst;
    qDebug() << "-- PIECE_LAST: " << pieceLast;
    qDebug() << "-- PIECE_TOTAL: " << pieceTotal;
    qDebug() << "-- PIECE_TOTAL_GLOBAL: " << pieceTotalGlobal;
    qDebug() << "-- PIECES_OFFSET_GLOBAL: " << pieceOffsetGlobal;
    qDebug() << "-- TOTAL_PRE_SKIP: " << skipGlobal;
    qDebug() << "-- OFFSET_PIECE_FILE: " << pieceOffset;
    
    for (int i=0; i < (pieceTotalGlobal+1); i++)
        if (i < pieceOffset)
            piecePriority.push_back(0);
        else if (i >= pieceOffset && i <= pieceLast)
            piecePriority.push_back(7);
        else
            piecePriority.push_back(0);


    //const qint64 remnant = pieceTotalGlobal-((offset_msec*pieceTotal)/total_msec);
    //if (((remnant*pieceLength)/MB) < 15)
        //required = remnant-1; //TODO: Cuando este solucionado el tema del slider, revisar esto.
    //else
        //required = 15;


    h.prioritize_pieces(piecePriority);
    dump();

}
*/















/*
for (int i=0; i < (pieceTotalGlobal+1); i++) {
    if (i < pieceOffset) {
        piecePriority.push_back(0);
    } else if (i >= pieceOffset && i <= pieceLast) {
        if (offsetPiecesLast_file != 0 && offsetPiecesLast_file > pieceOffset) {
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
offsetPiecesLast_file = pieceOffset;
*/



/*
void Torrent::progress()
{
    if (!aborted) {
        if (!skip) {
            const qint64 kb_downloaded = offset+(h.status().total_wanted_done/1024);
            //(*_player)->progress(total, kb_downloaded, 220);
        } else {
            const int downloadedSkip_mb = (h.status().total_done/MB)-skipGlobal;
            //(*_player)->progress(0, 0, downloadedSkip_mb);
        }
        QTimer::singleShot(1000, this, SLOT(progress()));
    }
}
*/
