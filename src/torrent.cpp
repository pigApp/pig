#include <QDebug>
#include <QDir>
#include <QTimer>

#include "torrent.h"

Torrent::Torrent(QObject *parent) : QObject(parent)
{
    client.listen_on(std::make_pair(6881, 6889), ec);
}

void Torrent::download(QString mangnetUrl)
{       
#ifdef _WIN32
        static std::string path = "C:/tmp/pig/";
#else
        static std::string path = "/tmp/pig/";
#endif
    params.save_path = path;
    params.url = mangnetUrl.toStdString();
    handle = client.add_torrent(params,  ec);
    handle.set_sequential_download(true);
    handle.set_priority(255);

    currentDownloadedPieces = 0;
    remap = true;
    QTimer::singleShot(30000, this, SLOT(controlPieces())); // Saber cuando descargo la metadata para llamar a controlPieces. status:: ...

    //if (torrent_status::downloading) {
    //    currentDownloadedPieces = 0;
    //    controlPieces();
    //}

    /*
    params.ti = new torrent_info(path.toStdString()+file.toStdString(), ec);
    handle.move_storage(params.save_path);

    nfo = new torrent_info(path.toStdString()+file.toStdString());

    for (int i=1; i<=scennes; i++)
        handle.file_priority(i, 0);
    handle.file_priority(scenne, 7);

    std::string origFileName = info->name(); // TODO: No devuelve el nombre de una fila individual.
    std::string format = origFileName.erase(0, origFileName.length()-4); //Ver si es necesario en windows.

    std::string ID = file.remove(".torrent", Qt::CaseSensitive).toStdString();
    std::string scenneID = QString::number(scenne).toStdString();
    std::string fileName = ID+"-"+scenneID;//+format;

    storage.rename_file(scenne, fileName);
    info->remap_files(storage);

    handle.rename_file(2, fileName);
    info->remap_files(storage);


    newFileName = QString::fromStdString(fileName);

    if (torrent_status::downloading) {
        currentDownloadedPieces = 0;
        controlPieces();
    }
    */
}

void Torrent::controlPieces()
{
    static int neededPices;

    if (remap) {
        static int lengthPieces;
        lengthPieces = handle.get_torrent_info().piece_length()/1024;
        if (lengthPieces >= 256 && lengthPieces < 512)
            neededPices = 30;
        else if (lengthPieces >= 512 && lengthPieces < 1024)
            neededPices = 15;
        else if (lengthPieces >= 1024 && lengthPieces < 2048)
            neededPices = 7;
        else if (lengthPieces >= 2048 && lengthPieces < 4096)
            neededPices = 2;//3
        else
            neededPices = 1;

        for (int i=1; i <= handle.get_torrent_info().num_files(); i++)
            handle.file_priority(i, 0); // TODO: filter_files.
        handle.file_priority(scenne, 7);

        remap = false;
    }

    _root->setProperty("neededPieces", neededPices);
    _root->setProperty("downloadedPieces", handle.status().num_pieces);
    _root->setProperty("bitRate", QString::number(handle.status().download_rate/1024));

    if(handle.status().num_pieces-currentDownloadedPieces < neededPices) {
        QTimer::singleShot(1000, this, SLOT(controlPieces()));
    } else {
        if (currentDownloadedPieces == 0) {
            static QString newFilePath;
            QString tmpPath = QString::fromStdString(handle.save_path())+QString::fromStdString(handle.name());
            QDir dir(tmpPath);
            if (dir.exists())
                newFilePath = tmpPath+"/";
            else
                newFilePath = QString::fromStdString(handle.save_path());
            QString newFileName = "Non.Stop.2014.720p.BluRay.x264.YIFY.mp4"; // TODO: Obtener nombre de la fila.
            QMetaObject::invokeMethod(_pig, "playerHandle", Qt::QueuedConnection, Q_ARG(QString, newFilePath), Q_ARG(QString, newFileName));
            QTimer::singleShot(5000, this, SLOT(progress()));;
        } else {
            QMetaObject::invokeMethod(_player, "update", Qt::QueuedConnection);
        }
    }

    /*
    int begin = handle.status().pieces.begin();
    int end = handle.status().pieces.end();
    int count = handle.status().pieces.count();

    qDebug() << "====================================";

    qDebug() << "PIECES-BEGIN: " << begin;
    qDebug() << "PIECES-END: " << end;
    qDebug() << "PIECES-COUNT: " << count;

    //qDebug() << "SIZE: " << info->file_at(0).size;

    int progress = handle.status().progress_ppm ;
    qDebug() << "PROGRESS: " << progress;

    if(progress < 50000)
        QTimer::singleShot(2000, this, SLOT(controlPieces()));
    else
        QMetaObject::invokeMethod(_pig, "playerHandle", Qt::QueuedConnection, Q_ARG(QString, pathx), Q_ARG(QString, filex));
    */
}

bool Torrent::availablePiece(int totalMsec, int offsetMsec)
{
    int offset = (offsetMsec*handle.get_torrent_info().num_pieces())/totalMsec;
    return handle.have_piece(offset);
}

void Torrent::offsetPiece(int totalMsec, int offsetMsec)
{
    int offset = (offsetMsec*handle.get_torrent_info().num_pieces())/totalMsec;
    qDebug() << "TOTAL_PIECES: " << handle.get_torrent_info().num_pieces();
    qDebug() << "OFFSET: " << offset;
    for (int i=0; i <= handle.get_torrent_info().num_pieces(); i++) { // TODO: Saber el numero de piezas de la fila a bajar. file_at.
        if(i < offset)
            handle.piece_priority(i, 0);
        else
          handle.piece_priority(i, 7);
    }
    currentDownloadedPieces = handle.status().num_pieces;
    controlPieces();
}

void Torrent::progress()
{
    // offset + ...
    QMetaObject::invokeMethod(_player, "progress", Qt::QueuedConnection, Q_ARG(int, handle.get_torrent_info().num_pieces()), Q_ARG(int, handle.status().num_pieces));
    QTimer::singleShot(5000, this, SLOT(progress()));
}















    /*
    if (ec)
    {
        fprintf(stderr, "failed to open listen socket: %s\n", ec.message().c_str());
        return 1;
    }

    p.ti = new torrent_info(argv[1], ec);
    if (ec)
    {
        fprintf(stderr, "%s\n", ec.message().c_str());
        return 1;
    }

    if (ec)
    {
        fprintf(stderr, "%s\n", ec.message().c_str());
        return 1;
    }
    */


    // std::vector<float> file_progress;
    // handle.file_progress(file_progress);



