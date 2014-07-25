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
    remap = true;
    skip = false;
    toWidget = false;
    offset = 0;
    downloadedPieces = 0;

#ifdef _WIN32
    std::string path = "C:/tmp/pig/";
#else
    QString home = QDir::homePath();
    std::string path = home.toStdString()+"/.pig/tmp/";
#endif
    params.save_path = path;
    params.url = mangnetUrl.toStdString();
    handle = client->add_torrent(params);

    ready_to_download(); 
}

void Torrent::ready_to_download()
{
    if (handle.status(1).state != 2) {
        minimum_pieces_required();
    } else if(!abort) {
        QTimer::singleShot(1000, this, SLOT(ready_to_download()));
    }
}

void Torrent::minimum_pieces_required()
{
    if (remap) {
        static QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264" << ".mkv" << ".mp4" << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
        bool checkScenneOffset = true;
        int scennesOffset = 1;
        int piece_kb = handle.get_torrent_info().piece_length()/1024;

        if (piece_kb >= 64 && piece_kb < 128)
            requiredPieces = 240;
        else if (piece_kb >= 128 && piece_kb < 256)
            requiredPieces = 120;
        else if (piece_kb >= 256 && piece_kb < 512)
            requiredPieces = 60;
        else if (piece_kb >= 512 && piece_kb < 1024)
            requiredPieces = 30;
        else if (piece_kb >= 1024 && piece_kb < 2048)
            requiredPieces = 15;
        else if (piece_kb >= 2048 && piece_kb < 4096)
             requiredPieces = 8;
        else if (piece_kb >= 4096 && piece_kb < 8192)
            requiredPieces = 4;
        else if (piece_kb >= 8192)
            requiredPieces = 2;

        files = handle.get_torrent_info().orig_files();
        for (int i=1; i<=handle.get_torrent_info().num_files(); i++) {
            if (checkScenneOffset) {
                fileName = QString::fromStdString(files.file_name(i-1));
                for (int f=0; f<formats.size(); ++f) {
                    if (fileName.endsWith(formats[f], Qt::CaseInsensitive)) {
                        if (scenne == scennesOffset) {
                            scenne = i;
                            checkScenneOffset = false;
                        } else {
                            ++scennesOffset;
                        }
                        break;
                    }
                }
            }
            handle.file_priority(i, 0);  // TODO: filter_files.
        }
        handle.file_priority(scenne, 7);
        //handle.set_upload_limit(100*1024);//
        handle.set_sequential_download(true);
        handle.set_priority(255);
        remap = false;
        
        download_Information();
    }

    qDebug() << "REQUEST__PIECES: " << requiredPieces;
    qDebug() << "DOWNLOAD_PIECES-DOWNLOAD_OFFSET: " << handle.status().num_pieces-downloadedPieces;
    qDebug() << "PIECE_KB: " << handle.get_torrent_info().piece_length()/1024;
    qDebug() << "DOWNLOADED: " << (handle.status().total_download/1048576)-3 << " MB";

    if (!abort) {
        if ((handle.status().total_download/1048576)-3 < 20) {  //== USAR MB EN LUGAR DE PIEZAS -le estoy restando 3M-
        //if (handle.status().num_pieces-downloadedPieces < requiredPieces) {
            QTimer::singleShot(1000, this, SLOT(minimum_pieces_required()));
        } else {
            call_player();
        }
    }
}

void Torrent::call_player()
{
    if (downloadedPieces == 0) {
        QDir dir(QString::fromStdString(handle.save_path())+QString::fromStdString(handle.name()));
        if (!dir.exists())
            dir.setPath(QString::fromStdString(handle.save_path()));
        QString absoluteFilePath = dir.absolutePath()+"/"+fileName; // TODO: Si la fila a descargar no incluye directorio, comprobar si dir.absolutePath agrega una barra al final.
        QMetaObject::invokeMethod(_pig, "player_handle", Qt::QueuedConnection, Q_ARG(QString, absoluteFilePath), Q_ARG(bool, false));
        toWidget = true;
        QTimer::singleShot(1000, this, SLOT(progress()));
    } else {
        QMetaObject::invokeMethod(_player, "update", Qt::QueuedConnection, Q_ARG(int, handle.get_torrent_info().num_pieces()), Q_ARG(int, offset));
        skip = false;
        QTimer::singleShot(7000, this, SLOT(progress()));
    }
}

void Torrent::progress()
{
    if (!abort) {
        int piece_kb = handle.get_torrent_info().piece_length()/1024;
        int total_kb = handle.get_torrent_info().total_size()/1024;  // TODO: Obtener el numero de bytes total, solo de la pieza actual.
        int downloaded_kb = handle.status().total_done/1024;
        QMetaObject::invokeMethod(_player, "progress", Qt::QueuedConnection, Q_ARG(int, piece_kb), Q_ARG(int, total_kb), Q_ARG(int, downloaded_kb));
        if (!skip)
            QTimer::singleShot(1000, this, SLOT(progress()));
    }
}

void Torrent::download_Information()
{
    if (!abort) {
        if (toWidget) {
            QMetaObject::invokeMethod(_player, "download_Information", Qt::QueuedConnection, Q_ARG(int, handle.status().download_rate/1024), Q_ARG(int, handle.status().num_peers));
        } else {
            int required_mbytes = 23;
            int downloaded_mbytes = handle.status().total_download/1048576;
            _root->setProperty("required", required_mbytes);
            _root->setProperty("downloaded", downloaded_mbytes);
            _root->setProperty("bitRate", QString::number(handle.status().download_rate/1024));
            _root->setProperty("peers", handle.status().num_peers);
        }
        QTimer::singleShot(1000, this, SLOT(download_Information()));
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

    downloadedPieces = handle.status().num_pieces;// TODO: En lugar de downloadedPieces y hacer la resta, usar total_wanted_done.(total descargado sin contar piezas con prioridad 0)
    minimum_pieces_required();
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
