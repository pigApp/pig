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

    // TODO: Nombre de la fila original(extenxion).
    std::string fileID = file.remove(".torrent", Qt::CaseSensitive).toStdString();
    std::string fileScenne = QString::number(scenne).toStdString();
    std::string fileName = fileID+"-"+fileScenne+".mkv";
    handle.rename_file(scenne-1, fileName);

    pieces = info->num_pieces(); // TODO: Saber el numero de piezas de la fila a bajar, no de todas las filas del .torrent.

    pathx = path;
    filex = QString::fromStdString(fileName);

    if (torrent_status::downloading)
        callPlayer();
}

void Torrent::callPlayer()
{
    //qDebug() << "SIZE: " << info->file_at(0).size;

    int progress = handle.status().progress_ppm ;
    qDebug() << "PROGRESS: " << progress;

    if(progress < 38637)
        QTimer::singleShot(2000, this, SLOT(sizeDownload()));
    else
        QMetaObject::invokeMethod(_pig, "openPlayer", Qt::QueuedConnection, Q_ARG(QString, pathx), Q_ARG(QString, filex));
}

void Torrent::offsetDownload(int offset)
{
    qDebug() << "TORRENT OFFSET: " << offset;

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



