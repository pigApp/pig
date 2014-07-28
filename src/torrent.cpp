#include "torrent.h"
#include <libtorrent/extensions/ut_pex.hpp>

#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QDebug>

Torrent::Torrent(QObject *parent) : QObject(parent)
{
    client = new libtorrent::session;
    client->listen_on(std::make_pair(6881, 6889), ec);
    client->start_dht();
    client->start_upnp();
    client->add_extension(&libtorrent::create_ut_pex_plugin);
}

Torrent::~Torrent()
{
    _pig->disconnect(this);
    _root->disconnect(this);
}

void Torrent::doConnect(QString mangnetUrl)
{
    abort = false;
    skip = false;
    toPlayer = false;
    offset = 0;

#ifdef _WIN32
    std::string path = "C:/tmp/pig/";
#else
    QString home = QDir::homePath();
    std::string path = home.toStdString()+"/.pig/tmp/";
#endif
    params.save_path = path;
    params.url = mangnetUrl.toStdString();
    params.file_priorities = 0;
    handle = client->add_torrent(params);
    handle.set_sequential_download(true);
    handle.set_priority(255);

    metadata_ready();
}

void Torrent::metadata_ready()
{
    if (handle.status(1).state != 2) {
       filterFile();
    } else if (!abort) {
        QTimer::singleShot(1000, this, SLOT(metadata_ready()));
    }
}

void Torrent::filterFile()
{
    static QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264" << ".mkv" << ".mp4" << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
    bool checkFile = true;
    int scennesOffset = 1;

    files = handle.get_torrent_info().orig_files();
    for (int i=1; i<=handle.get_torrent_info().num_files(); i++) {
        if (checkFile) {
            fileName = QString::fromStdString(files.file_name(i-1));
            for (int f=0; f<formats.size(); ++f) {
                if (fileName.endsWith(formats[f], Qt::CaseInsensitive)) {
                    if (scenne == scennesOffset) {
                        scenne = i;
                        checkFile = false;
                    } else {
                        ++scennesOffset;
                    }
                    break;
                }
            }
        } else {
            break;
        }
    }
    handle.file_priority(scenne, 7); // TODO: No funciona.
    minimum_ready();
    download_Information();
}

void Torrent::minimum_ready()
{
    if (!abort) {
        if ((handle.status().total_wanted_done/1048576) < 19) {  // TODO: Obetener la cantidad de megas minimos necesarios con un porcentaje del peso del archivo.
            QTimer::singleShot(1000, this, SLOT(minimum_ready()));
        } else {
            call_player();
        }
    }
    qDebug() << "DOWNLOADED_PIECES: " << handle.status().num_pieces;
    qDebug() << "DOWNLOADED: " << (handle.status().total_wanted_done/1048576) << " MB";
}

void Torrent::download_Information()
{
    if (!abort) {
        if (toPlayer) {
            QMetaObject::invokeMethod(_player, "download_Information", Qt::QueuedConnection, Q_ARG(int, handle.status().download_rate/1024), Q_ARG(int, handle.status().num_peers));
        } else {
            int required_mbytes = 19;
            int downloaded_mbytes = handle.status().total_wanted_done/1048576;
            _root->setProperty("required", required_mbytes);
            _root->setProperty("downloaded", downloaded_mbytes);
            _root->setProperty("bitRate", QString::number(handle.status().download_rate/1024));
            _root->setProperty("peers", handle.status().num_peers);
        }
        QTimer::singleShot(1000, this, SLOT(download_Information()));
    }
}

void Torrent::call_player()
{
    if (!skip) {
        QDir dir(QString::fromStdString(handle.save_path())+QString::fromStdString(handle.name()));
        if (!dir.exists())
            dir.setPath(QString::fromStdString(handle.save_path()));
        QString absoluteFilePath = dir.absolutePath()+"/"+fileName; 
        QMetaObject::invokeMethod(_pig, "player_handle", Qt::QueuedConnection, Q_ARG(QString, absoluteFilePath), Q_ARG(bool, true), Q_ARG(bool, true), Q_ARG(bool, false), Q_ARG(bool, false));
        QTimer::singleShot(3000, this, SLOT(progress()));
    } else {
        QMetaObject::invokeMethod(_player, "update", Qt::QueuedConnection, Q_ARG(int, handle.get_torrent_info().num_pieces()), Q_ARG(int, offset));
        QTimer::singleShot(7000, this, SLOT(progress()));
        skip = false;
    }
}

void Torrent::progress()
{
    if (!abort) {
        int total_kb = handle.get_torrent_info().total_size()/1024;  // TODO: Obtener el numero de bytes total, solo de la pieza actual.
        int downloaded_kb = handle.status().total_wanted_done/1024;  //TODO: Sumarle offset.
        QMetaObject::invokeMethod(_player, "progress", Qt::QueuedConnection, Q_ARG(int, total_kb), Q_ARG(int, downloaded_kb));
        if (!skip)
            QTimer::singleShot(1000, this, SLOT(progress()));
    }
}

bool Torrent::piece_is_available(int total_msec, int offset_msec,int piece)
{
    if (piece == 0) {
        offset = (offset_msec*handle.get_torrent_info().num_pieces())/total_msec;
        return handle.have_piece(offset);
    } else {
        return handle.have_piece(piece);
    }
}

void Torrent::piece_offset(int total_msec, int offset_msec) // TODO: Si se empieza a  bajar desde una pieza que no se tiene, poner disable el slider en videoplayer.
{
    skip = true;
    offset = (offset_msec*handle.get_torrent_info().num_pieces())/total_msec;

    for (int i=0; i <= handle.get_torrent_info().num_pieces(); i++) { // TODO: Saber el numero de piezas de la fila a bajar. file_at.
        if(i < offset)
            handle.piece_priority(i, 0);
        else
            handle.piece_priority(i, 7);
    }
    minimum_ready();
}

void Torrent::stop()
{
    abort = true;

    if (handle.status().has_metadata) {
        client->remove_torrent(handle, optind = client->delete_files);
        _player->disconnect();
    } else {
        // TODO: Detener el torrent sin borrar filas.
    }
}
