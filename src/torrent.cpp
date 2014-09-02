#include "torrent.h"

#ifdef _WIN32
    #include "..\lib\libtorrent-rasterbar-1.0.1\include\libtorrent\extensions\ut_pex.hpp"
#else
    #include <libtorrent/extensions/ut_pex.hpp>
#endif

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

void Torrent::doConnect(QString *magnet)
{
    abort = false;
    skip = false;
    toPlayer = false;
    minimum_mb = 15;
    pieceLength = 0;
    firstPiece_file = 0;
    lastPiece_file = 0;
    totalPieces_file = 0;
    offsetPieces_file = 0;
    offsetPieces = 0;
    totalPieces = 0;
    total_kb = 0;
    offset_kb = 0;
    _player = NULL;

#ifdef _WIN32
    const std::string path = "C:/tmp/pig/";
#else
    const QString home = QDir::homePath();
    const std::string path = home.toStdString()+"/.pig/tmp/";
#endif
    params.save_path = path;
    params.url = magnet->toStdString();
    handle = client->add_torrent(params);
    handle.set_sequential_download(true);
    handle.set_priority(255);

    metadata_ready();
}

void Torrent::metadata_ready()
{
    if (handle.status(1).state != 2) {
       filter_files();
    } else if (!abort) {
        QTimer::singleShot(1000, this, SLOT(metadata_ready()));
    }
}

void Torrent::filter_files()
{
    QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264" << ".mkv" << ".mp4" << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
    std::vector<int> filePriority;
    file_storage = handle.get_torrent_info().orig_files();
    bool checkFile = true;
    const int numberOfFiles = handle.get_torrent_info().num_files();
    int scennesOffset = 1;

    filePriority.clear();
    for (int i=1; i<=numberOfFiles; i++) {
        if (checkFile) {
            fileName = QString::fromStdString(file_storage.file_name(i-1));
            for (int f=0; f<formats.size(); ++f) {
                if (fileName.endsWith(formats[f], Qt::CaseInsensitive)) {
                    if (scene == scennesOffset) {
                        filePriority.push_back(7);
                        scene = i;
                        checkFile = false;
                    } else {
                        filePriority.push_back(0);
                        ++scennesOffset;
                    }
                    break;
                } else {
                    if (f == formats.size()-1)
                        filePriority.push_back(0);
                }
            }
        } else {
            filePriority.push_back(0);
        }
    }
    handle.prioritize_files(filePriority); 

    file_storage = handle.get_torrent_info().files();
    pieceLength = file_storage.piece_length();
    qDebug() << "-- PIECE_LENGTH: " << pieceLength/1024;
    firstPiece_file = file_storage.map_file(scene-1, 0, 0).piece;
    lastPiece_file = file_storage.map_file(scene-1, file_storage.file_size(scene-1), 0).piece;
    totalPieces_file = lastPiece_file - firstPiece_file;
    totalPieces = handle.get_torrent_info().num_pieces();
    total_kb = (totalPieces_file*pieceLength)/1024;
    for (int i=0; i<(scene-1); i++ )
        offsetPieces = offsetPieces+(file_storage.map_file(i, file_storage.file_size(i), 0).piece - file_storage.map_file(i, 0, 0).piece);
        
    minimum_ready();
    download_Information();
}

void Torrent::minimum_ready()
{
    if (!abort) {
        if ((handle.status().total_wanted_done/1048576) < minimum_mb) {
            QTimer::singleShot(1000, this, SLOT(minimum_ready()));
        } else {
            call_player();
        }
    }
    qDebug() << "DOWNLOADED: " << (handle.status().total_wanted_done/1048576) << " MB";
}

void Torrent::download_Information()
{
    if (!abort) {
        const int bitRate = handle.status().download_rate/1024;
        const int peers = handle.status().num_peers;
        if (toPlayer) {
            QMetaObject::invokeMethod(_player, "download_Information", Qt::QueuedConnection, Q_ARG(int, bitRate), Q_ARG(int, peers));
        } else {
            const int downloaded_mb = handle.status().total_wanted_done/1048576;
            _root->setProperty("required", 15);
            _root->setProperty("downloaded", downloaded_mb);
            _root->setProperty("bitRate", QString::number(bitRate));
            _root->setProperty("peers", peers);
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
        const QString absoluteFilePath = dir.absolutePath()+"/"+fileName;
        QMetaObject::invokeMethod(_pig, "player_handle", Qt::QueuedConnection, Q_ARG(const QString, absoluteFilePath), Q_ARG(bool, true), Q_ARG(bool, true), Q_ARG(bool, false), Q_ARG(bool, false));
        QTimer::singleShot(1000, this, SLOT(progress()));
    } else {
        skip = false;
        QMetaObject::invokeMethod(_player, "update_player", Qt::DirectConnection);
    }
}

void Torrent::progress()
{
    if (!abort) {
        if (!skip) {
            const qint64 downloaded_kb = offset_kb+(handle.status().total_wanted_done/1024);
            QMetaObject::invokeMethod(_player, "progress", Qt::QueuedConnection, Q_ARG(qint64, total_kb), Q_ARG(qint64, downloaded_kb), Q_ARG(int, 220));
        } else {
            const int downloaded_offset = handle.status().total_wanted_done/1048576;
            QMetaObject::invokeMethod(_player, "progress", Qt::QueuedConnection, Q_ARG(qint64, 0), Q_ARG(qint64, 0), Q_ARG(int, downloaded_offset));
        }
        QTimer::singleShot(1000, this, SLOT(progress()));
    }
}

bool Torrent::piece_is_available(qint64 total_msec, qint64 offset_msec)
{
    offsetPieces_file = offsetPieces+((offset_msec*totalPieces_file)/total_msec);
    return handle.have_piece(offsetPieces_file);
}

void Torrent::piece_update(qint64 total_msec, qint64 offset_msec)
{
    std::vector<int> piecePriority;
    skip = true;

    offsetPieces_file = offsetPieces+(((offset_msec+fit)*totalPieces_file)/total_msec); 
    offset_kb = (offsetPieces+((offset_msec*totalPieces_file)/total_msec))*(pieceLength/1024);

    qDebug() << "-- SCENE: " << scene;
    qDebug() << "-- TOTAL_MSEC: " << total_msec;
    qDebug() << "-- OFFSET_MESEC: " << offset_msec;
    qDebug() << "-- FIRST_PIECE_FILE: " << firstPiece_file;
    qDebug() << "-- LAST_PIECE_FILE: " << lastPiece_file;
    qDebug() << "-- TOTAL_PIECE_FILE: " << totalPieces_file;
    qDebug() << "-- TOTAL_PIECE: " << totalPieces;
    qDebug() << "-- OFFSET_PIECES: " << offsetPieces;
    qDebug() << "-- OFFSET_PIECE_FILE: " << offsetPieces_file;
    
    //piecePriority.clear();
    std::fill(piecePriority.begin(), piecePriority.end(), 0);
    for (int i=0; i < (totalPieces+1); i++)
        if(i < offsetPieces_file)
            piecePriority.push_back(0);
        else if (i >= offsetPieces_file && i <= lastPiece_file)
            piecePriority.push_back(7);
        else
            piecePriority.push_back(0);

    // TODO: Falla al mover el slider hacia atras. LLama al reproductor directamente porque ve que ya bajo mas de 15 megas. Debe estar asignando mal la prioridad de las piezas. Ver con un loop la prioridad de la piezas.

    // TODO: Falla al sumar fit. Si se mueve el slider a una posicion mayor que una hora, funciona bien. Si es menor, comienza a descargar desde un poco mas adelante. Menor es el msec al que se mueve el slider mayor es el margen. Posible solucion: Calcular el porcentaje de pelicula donde cae el slider y restarle a fit de acuerdo a eso.

    /*
    const qint64 remnant = totalPieces-((offset_msec*totalPieces_file)/total_msec);
    if (((remnant*pieceLength)/1048576) < 15)
        minimum_mb = remnant-1; //TODO: Cuando este solucionado el tema del slider, revisar esto.
    else 
        minimum_mb = 15;
    */

    handle.prioritize_pieces(piecePriority);
    minimum_ready();
}

void Torrent::stop()
{
    abort = true;
    if (handle.status().has_metadata) {
        client->remove_torrent(handle);
        if (_player != NULL)
            _player->disconnect();
    } else {
        // TODO: Detener el torrent de una manera que no necesite el handle. Si no se tiene la metadata el handle es invalido y falla. 
    }
}
