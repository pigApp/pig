#include "torrent.h"

#include <libtorrent/file_storage.hpp>
#include <libtorrent/extensions/ut_pex.hpp>

#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QDebug>

Torrent::Torrent(QObject *parent) : QObject(parent)
{    
    client.listen_on(std::make_pair(6881, 6889), ec);
    client.start_dht();
    client.start_upnp();
    client.add_extension(&libtorrent::create_ut_pex_plugin);
}

Torrent::~Torrent()
{
    client.abort();
    _pig->disconnect();
    _root->disconnect();
}

void Torrent::doRun(QString mangnetUrl)
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
    handle = client.add_torrent(params);

    //std::string tracker = "udp://tracker.publicbt.com:80/announce";
    //std::string trackerTwo = "udp://tracker.istole.it:80/announce";
    //handle.add_tracker(tracker);
    //handle.add_tracker(trackerTwo);

    metadataReady(); // TODO: En vez de llamar a esta funcion usar la alerta de metadata descargada.
}

void Torrent::metadataReady()
{
    if (handle.status(1).state != 2) {
        minimumPiecesRequired();
    } else if(!abort) {
        QTimer::singleShot(1000, this, SLOT(metadataReady()));
    }
}

void Torrent::minimumPiecesRequired()
{
    if (remap) {
        piece_kb = handle.get_torrent_info().piece_length()/1024;

        if (piece_kb < 256)
            requiredPieces = 60;
        else if (piece_kb >= 256 && piece_kb < 512)
            requiredPieces = 45;
        else if (piece_kb >= 512 && piece_kb < 1024)
            requiredPieces = 25;
        else if (piece_kb >= 1024 && piece_kb < 2048)
            requiredPieces = 15;
        else if (piece_kb >= 2048 && piece_kb < 4096)
            requiredPieces = 6;
        else
            requiredPieces = 3;

        required_kb = requiredPieces*piece_kb*1024; // TODO: Agregar o sumar una pieza segun lo que pese. Sino muestra mal la barra de progreso.

        libtorrent::file_storage fs = handle.get_torrent_info().orig_files();
        std::string nm = fs.name();
        std::cout << nm;

        for (int i=1; i<=handle.get_torrent_info().num_files(); i++)
            handle.file_priority(i, 0); // TODO: filter_files.
        handle.file_priority(scenne, 7); // TODO: Saber el nombre de las filas, filtradas por formato.
        //handle.set_upload_limit(100*1024);//

        handle.set_sequential_download(true);
        handle.set_priority(255);
        remap = false;
        
        downloadInfo();
    }

    qDebug() << "REQUEST__PIECES: " << requiredPieces;
    qDebug() << "DOWNLOAD_PIECES-DOWNLOAD_OFFSET: " << handle.status().num_pieces-downloadedPieces;

    if (!abort) {
        if(handle.status().num_pieces-downloadedPieces < requiredPieces) {
            QTimer::singleShot(1000, this, SLOT(minimumPiecesRequired()));
        } else {
            if (downloadedPieces == 0) {
                static QStringList scennesAbsolutePath;

                QString dirPath = QString::fromStdString(handle.save_path())+QString::fromStdString(handle.name());
                QDir dir(dirPath);
                if (!dir.exists())
                    dir.setPath(QString::fromStdString(handle.save_path()));

                dir.setNameFilters(QStringList() << "*.avi" << "*.divx" << "*.flv" << "*.h264" << "*.mkv" << "*.mp4" << "*.mpg" << "*.mpeg" << "*.ogm"<< "*.ogv" << "*.wmv");
                QFileInfoList filesList(dir.entryInfoList(QDir::Files, QDir::Name));
                foreach(const QFileInfo & fi, filesList )
                    scennesAbsolutePath << fi.absoluteFilePath();

                toWidget = true;
                QMetaObject::invokeMethod(_pig, "playerHandle", Qt::QueuedConnection, Q_ARG(QString, scennesAbsolutePath[scenne-1]), Q_ARG(bool, false));

                QTimer::singleShot(1000, this, SLOT(progress()));
            } else {
                QMetaObject::invokeMethod(_player, "update", Qt::QueuedConnection, Q_ARG(int, handle.get_torrent_info().num_pieces()), Q_ARG(int, offset));
                //skip = false;
                //QTimer::singleShot(7000, this, SLOT(progress()));
            }
        }
    }
}

void Torrent::progress()
{
    int total_kb = handle.get_torrent_info().total_size()/1024;  // TODO: Obtener el numero de bytes total, solo de la pieza actual.
    int downloaded_kb = handle.status().total_done/1024; 

    QMetaObject::invokeMethod(_player, "progress", Qt::QueuedConnection, Q_ARG(int, piece_kb), Q_ARG(int, total_kb), Q_ARG(int, downloaded_kb));
    if (!skip)
        QTimer::singleShot(1000, this, SLOT(progress()));
}

void Torrent::downloadInfo()
{
    if (toWidget) {
        QMetaObject::invokeMethod(_player, "downloadInfo", Qt::QueuedConnection, Q_ARG(int, handle.status().download_rate/1024), Q_ARG(int, handle.status().num_peers));
    } else {
        int downloaded_kb = handle.status().total_done;
        _root->setProperty("required", required_kb);
        _root->setProperty("downloaded", downloaded_kb);
        _root->setProperty("bitRate", QString::number(handle.status().download_rate/1024));
        _root->setProperty("peers", handle.status().num_peers);
    }

    if (!abort)
        QTimer::singleShot(1000, this, SLOT(downloadInfo()));
}

bool Torrent::isAvailable(int total_msec, int offset_msec,int availablePiece)
{
    if (availablePiece == 0) {
        offset = (offset_msec*handle.get_torrent_info().num_pieces())/total_msec;
        return handle.have_piece(offset);
    } else {
        return handle.have_piece(availablePiece);
    }
}

void Torrent::offsetPiece(int total_msec, int offset_msec) // TODO: Si se empieza a  bajar desde una pieza que no se tiene, poner disable el slider en videoplayer.
{
    skip = true;
    offset = (offset_msec*handle.get_torrent_info().num_pieces())/total_msec;

    for (int i=0; i <= handle.get_torrent_info().num_pieces(); i++) { // TODO: Saber el numero de piezas de la fila a bajar. file_at.
        if(i < offset)
            handle.piece_priority(i, 0);
        else
            handle.piece_priority(i, 7);
    }

    downloadedPieces = handle.status().num_pieces;
    minimumPiecesRequired();
}

void Torrent::stop()
{
    if (handle.status().has_metadata) { // TODO: Al terminar de descargar la metadata vuelve a dar invalido. Agregar (handle.status().has_metadata || el archivo se esta escribiendo)
        client.remove_torrent(handle, optind = client.delete_files);
    } else {
        // TODO: Detener el torrent sin remove_torrent.
    }
    abort = true;
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
    downloadOffsetPieces = 0;
    minimumPiecesReady();
}
*/

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
    QTimer::singleShot(2000, this, SLOT(minimumPiecesReady()));
else
    QMetaObject::invokeMethod(_pig, "playerHandle", Qt::QueuedConnection, Q_ARG(QString, pathx), Q_ARG(QString, filex));
*/
