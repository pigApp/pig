#include <QDebug>
#include <QTimer>

#include "torrent.h"

Torrent::Torrent(QObject *parent, QObject *obj) : QObject(parent)
{
    client.listen_on(std::make_pair(6881, 6889), ec);
    _pig = obj;
}

void Torrent::download(QString path, QString file, int scenne)
{       
    params.save_path = path.toStdString() ;
    params.ti = new torrent_info(path.toStdString()+file.toStdString(), ec); 

    handle = client.add_torrent(params, ec);
    handle.set_sequential_download(true);

    info = new torrent_info(path.toStdString()+file.toStdString());

    int scennes = info->num_files();
    for (int i=1; i<=scennes; i++)
        handle.file_priority(i, 0);
    handle.file_priority(scenne, 7);
    handle.set_priority(255);

    std::string origFileName = info->name(); // TODO: No devuelve el nombre de una fila individual.
    std::string ID = file.remove(".torrent", Qt::CaseSensitive).toStdString();
    std::string scenneID = QString::number(scenne).toStdString();
    std::string format = origFileName.erase(0, origFileName.length()-4);
    std::string newFileName = ID+"-"+scenneID+format;
    handle.rename_file(scenne-1, newFileName);

    pathx = path;
    filex = QString::fromStdString(newFileName);
    if (torrent_status::downloading)
        neededPiecesToPlay();
}

void Torrent::neededPiecesToPlay()
{
    int neededPices;
    int lengthPieces = info->piece_length()/1024;
    int downloadedPieces = handle.status().num_pieces;

    if (lengthPieces >= 256 && lengthPieces < 512)
        neededPices = 25;
    else if (lengthPieces >= 512 && lengthPieces < 1024)
        neededPices = 13;
    else if (lengthPieces >= 1024 && lengthPieces < 2048)
        neededPices = 6;
    else if (lengthPieces >= 2048 && lengthPieces < 4096)
        neededPices = 3;
    else
        neededPices = 1;

    qDebug() << "PIECES-NEEDED: " << neededPices;
    qDebug() << "PIECES-LENGTH: " << lengthPieces;
    qDebug() << "DOWNLOADED-PIECES: " << downloadedPieces;

    int download_rate = handle.status().download_rate/1024;
    qDebug() << "BitRate: " << download_rate <<"/KBs";

    /*
    int begin = handle.status().pieces.begin();
    int end = handle.status().pieces.end();
    int count = handle.status().pieces.count();

    qDebug() << "====================================";

    qDebug() << "PIECES-BEGIN: " << begin;
    qDebug() << "PIECES-END: " << end;
    qDebug() << "PIECES-COUNT: " << count;
    */

    if(downloadedPieces < neededPices)
        QTimer::singleShot(2000, this, SLOT(neededPiecesToPlay()));
    else
        QMetaObject::invokeMethod(_pig, "playerHandle", Qt::QueuedConnection, Q_ARG(QString, pathx), Q_ARG(QString, filex));

    /*
    //qDebug() << "SIZE: " << info->file_at(0).size;

    int progress = handle.status().progress_ppm ;
    qDebug() << "PROGRESS: " << progress;

    if(progress < 50000)
        QTimer::singleShot(2000, this, SLOT(neededPiecesToPlay()));
    else
        QMetaObject::invokeMethod(_pig, "playerHandle", Qt::QueuedConnection, Q_ARG(QString, pathx), Q_ARG(QString, filex));
    */
}

bool Torrent::availablePiece(int piece)
{
    return handle.have_piece(piece);
}

void Torrent::offsetPiece(int offset)
{
    int pieces = info->num_pieces(); // TODO: Saber el numero de piezas de la fila a bajar, no de todas las filas del .torrent.

    qDebug() << "TOTAL PIECES: " << pieces;
    qDebug() << "PIECE TO DOWNLOAD: " << offset;

    for (int i=0; i<=pieces; i++) {
        if(i < offset)
            handle.piece_priority(i, 0);
        else
          handle.piece_priority(i, 7);
    }
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



