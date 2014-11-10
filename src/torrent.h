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
    explicit Torrent(QObject *parent=0);
    ~Torrent();

    Q_INVOKABLE bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    Q_INVOKABLE void piece_update(qint64 total_msec, qint64 offset_msec);

    QObject *_player;
    QObject *_root;

    bool toPlayer;
    int scene;

public slots:
    void start(const QString *magnet);
    void stop();

signals:
    void sandbox_signal(const QString absoluteFilePath, bool sandbox, bool fileReady, bool close);

private:
    libtorrent::session *client;
    libtorrent::torrent_handle handler;
    libtorrent::add_torrent_params params;
    libtorrent::file_storage file_storage;
    libtorrent::error_code ec;

    QString fileName;

    bool abort, skip;
    int minimum_mb; 
    qint64 pieceLength, firstPiece_file, lastPiece_file, totalPieces_file, offsetPieces_file,
           offsetPieces, totalPieces, total_kb, totalPreSkip_mb, offset_kb;

private slots:
    void metadata_success();
    void filter_files();
    void minimum_success();
    void download_Information();
    void call_player();
    void progress();
};

#endif
