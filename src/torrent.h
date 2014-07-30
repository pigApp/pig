    #ifndef TORRENT_H
#define TORRENT_H

#include <libtorrent/session.hpp>
#include <libtorrent/file_storage.hpp>

#include <stdlib.h>
#include <iostream>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent=0);
    ~Torrent();

    Q_INVOKABLE bool piece_is_available(int total_msec, int offset_msec);
    Q_INVOKABLE void piece_update(int total_msec, int offset_msec);

    QObject *_pig;
    QObject *_player;
    QObject *_root;

    int scenne;
    bool toPlayer;

public slots:
    void doConnect(QString magnet);
    void stop();

private:
    libtorrent::session *client;
    libtorrent::torrent_handle handle;
    libtorrent::add_torrent_params params;
    libtorrent::file_storage files;
    libtorrent::error_code ec;

    QString fileName;

    bool abort;
    bool skip;
    int offset;
    int offset_kb;

private slots:
    void metadata_ready();
    void filterFile();
    void minimum_ready();
    void download_Information();
    void call_player();
    void progress();
};

#endif
