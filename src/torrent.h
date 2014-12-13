#ifndef TORRENT_H
#define TORRENT_H

#include <libtorrent/session.hpp>
#include <libtorrent/file_storage.hpp>

#include <stdlib.h>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent=0, const QString *magnet=NULL);
    ~Torrent();

    QObject **_root;

    int scene;

public slots:
    bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    void piece_update(qint64 total_msec, qint64 offset_msec);

private:
    libtorrent::session *client;
    libtorrent::torrent_handle handler;
    libtorrent::add_torrent_params params;
    libtorrent::file_storage file_storage;
    libtorrent::error_code ec;

    QString fileName;
    qint64 pieceLength;
    qint64 firstPiece_file, lastPiece_file, totalPieces_file, offsetPieces_file;
    qint64 offsetPieces, totalPieces;
    qint64 total_kb, totalPreSkip_mb, offset_kb;

    bool aborted, skip;
    int minimum_mb; 

private slots:
    void get_metadata();
    void filter_files();
    void required_video_dump();
    void download_Information();
    void call_player();
    void progress();
};

#endif
