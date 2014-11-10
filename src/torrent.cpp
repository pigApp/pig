#include "torrent.h"

#include <libtorrent/extensions/ut_pex.hpp>

#include <QDir>
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
    _root->disconnect(this);
}

void Torrent::start(const QString *magnet)
{
    _player = NULL;
    abort = false;
    skip = false;
    toPlayer = false;
    minimum_mb = 15;
    totalPreSkip_mb = 0;
    pieceLength = 0;
    firstPiece_file = 0;
    lastPiece_file = 0;
    totalPieces_file = 0;
    offsetPieces_file = 0;
    offsetPieces = 0;
    totalPieces = 0;
    total_kb = 0;
    offset_kb = 0;

#ifdef _WIN32
    const std::string path = "C:/PIG/.pig/tmp/";
#else
    const QString home = QDir::homePath();
    const std::string path = home.toStdString()+"/.pig/tmp/";
#endif
    params.save_path = path;
    params.url = magnet->toStdString();
    handler = client->add_torrent(params);
    handler.set_sequential_download(true);
    handler.set_priority(255);

    metadata_success();
}

void Torrent::metadata_success()
{
    if (!abort) {
        if (handler.status(1).state != 2)
           filter_files();
        else
            QTimer::singleShot(1000, this, SLOT(metadata_success()));
    }
}

void Torrent::filter_files()
{
    QStringList formats; formats << ".avi" << ".divx" << ".flv" << ".h264" << ".mkv" << ".mp4" << ".mpg" << ".mpeg" << ".ogm"<< ".ogv" << ".wmv";
    std::vector<int> filePriority;
    file_storage = handler.get_torrent_info().orig_files();
    bool checkFile = true;
    const int numberOfFiles = handler.get_torrent_info().num_files();
    int scennesOffset = 1;

    filePriority.clear();
    for (int i=1; i <= numberOfFiles; i++) {
        if (checkFile) {
            fileName = QString::fromStdString(file_storage.file_name(i-1));
            for (int f=0; f < formats.size(); ++f) {
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
    handler.prioritize_files(filePriority); 

    file_storage = handler.get_torrent_info().files();
    pieceLength = file_storage.piece_length();
    qDebug() << "-- PIECE_LENGTH: " << pieceLength/1024;
    firstPiece_file = file_storage.map_file(scene-1, 0, 0).piece;
    lastPiece_file = file_storage.map_file(scene-1, file_storage.file_size(scene-1), 0).piece;
    totalPieces_file = lastPiece_file - firstPiece_file;
    totalPieces = handler.get_torrent_info().num_pieces();
    total_kb = (totalPieces_file*pieceLength)/1024;
    for (int i=0; i < (scene-1); i++ )
        offsetPieces = offsetPieces+(file_storage.map_file(i, file_storage.file_size(i), 0).piece - file_storage.map_file(i, 0, 0).piece);
        
    minimum_success();
    download_Information();
}

void Torrent::minimum_success()
{
    if (!abort) {
        if (((handler.status().total_done/1048576)-totalPreSkip_mb) < minimum_mb)
            QTimer::singleShot(1000, this, SLOT(minimum_success()));
        else 
            call_player();
    }

    qDebug() << "DOWNLOADED: " << (handler.status().total_done/1048576)-totalPreSkip_mb << " MB";
}

void Torrent::download_Information()
{
    if (!abort) {
        const int bitRate = handler.status().download_rate/1024;
        const int peers = handler.status().num_peers;
        if (toPlayer) {
            QMetaObject::invokeMethod (_player, "download_Information", Qt::QueuedConnection, Q_ARG(int, bitRate), Q_ARG(int, peers));
        } else {
            const int downloaded_mb = handler.status().total_wanted_done/1048576;
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
        QDir dir(QString::fromStdString(handler.save_path())+QString::fromStdString(handler.name()));
        if (!dir.exists())
            dir.setPath(QString::fromStdString(handler.save_path()));
        const QString absoluteFilePath = dir.absolutePath()+"/"+fileName;

        emit sandbox_signal(absoluteFilePath, true, false, false);
        QTimer::singleShot(1000, this, SLOT(progress()));
    } else {
        skip = false;
        QMetaObject::invokeMethod (_player, "update_player", Qt::DirectConnection);
    }
}

void Torrent::progress()
{
    if (!abort) {
        if (!skip) {
            const qint64 downloaded_kb = offset_kb+(handler.status().total_wanted_done/1024);
            QMetaObject::invokeMethod (_player, "progress", Qt::QueuedConnection, Q_ARG(qint64, total_kb), Q_ARG(qint64, downloaded_kb), Q_ARG(int, 220));
        } else {
            const int downloadedSkip_mb = (handler.status().total_done/1048576)-totalPreSkip_mb;
            QMetaObject::invokeMethod (_player, "progress", Qt::QueuedConnection, Q_ARG(qint64, 0), Q_ARG(qint64, 0), Q_ARG(int, downloadedSkip_mb));
        }
        QTimer::singleShot(1000, this, SLOT(progress()));
    }
}

bool Torrent::piece_is_available(qint64 total_msec, qint64 offset_msec)
{
    //SIZE
    //qDebug() << "FILE_SIZE_TORRENT: " << handler.get_torrent_info().total_size();

    offsetPieces_file = offsetPieces+((offset_msec*totalPieces_file)/total_msec);
    return handler.have_piece(offsetPieces_file);
}

void Torrent::piece_update(qint64 total_msec, qint64 offset_msec)
{
    std::vector<int> piecePriority;
    skip = true;

    //offsetPieces_file = 475; //offsetPieces+(((offset_msec)*totalPieces_file)/total_msec);
    offsetPieces_file = offsetPieces+((99*totalPieces_file)/100);
    offset_kb = (offsetPieces+((offset_msec*totalPieces_file)/total_msec))*(pieceLength/1024);
    totalPreSkip_mb = handler.status().total_done/1048576;

    qDebug() << "-- SCENE: " << scene;
    qDebug() << "-- TOTAL_MSEC: " << total_msec;
    qDebug() << "-- OFFSET_MESEC: " << offset_msec;
    qDebug() << "-- FIRST_PIECE_FILE: " << firstPiece_file;
    qDebug() << "-- LAST_PIECE_FILE: " << lastPiece_file;
    qDebug() << "-- TOTAL_PIECE_FILE: " << totalPieces_file;
    qDebug() << "-- TOTAL_PIECE: " << totalPieces;
    qDebug() << "-- TOTAL_PRE_SKIP: " << totalPreSkip_mb;
    qDebug() << "-- OFFSET_PIECES: " << offsetPieces;
    qDebug() << "-- OFFSET_PIECE_FILE: " << offsetPieces_file;
    
    for (int i=0; i < (totalPieces+1); i++)
        if (i < offsetPieces_file) 
            piecePriority.push_back(0);
        else if (i >= offsetPieces_file && i <= lastPiece_file) 
            piecePriority.push_back(7);
        else
            piecePriority.push_back(0);

    /*
    const qint64 remnant = totalPieces-((offset_msec*totalPieces_file)/total_msec);
    if (((remnant*pieceLength)/1048576) < 15)
        minimum_mb = remnant-1; //TODO: Cuando este solucionado el tema del slider, revisar esto.
    else 
        minimum_mb = 15;
    */

    handler.prioritize_pieces(piecePriority);
    minimum_success();
}

void Torrent::stop()
{
    abort = true;
    client->remove_torrent(handler);
    if (_player != NULL)
        _player->disconnect();
}









/*
for (int i=0; i < (totalPieces+1); i++) {
    if (i < offsetPieces_file) {
        piecePriority.push_back(0);
    } else if (i >= offsetPieces_file && i <= lastPiece_file) {
        if (offsetPiecesLast_file != 0 && offsetPiecesLast_file > offsetPieces_file) {
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
offsetPiecesLast_file = offsetPieces_file;
*/
