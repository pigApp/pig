#ifndef TORRENT_H
#define TORRENT_H

#include "tcpSocket.h"
#include "player.h"

//#include <libtorrent/torrent.hpp>//
#include <libtorrent/session.hpp>
//  #include <libtorrent/disk_io_thread.hpp>

#include <stdlib.h>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent = 0, QObject **root = NULL, const int * const scene = NULL);
    ~Torrent();

public slots:
    void get(const QString * const host, const QString * const url, const QString * const target);

    //bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    //void piece_update(qint64 total_msec, qint64 offset_msec);

private:
    QObject **_root;
    TcpSocket *mSocket;//
    //libtorrent::torrent::request_time_critical_pieces();
    libtorrent::session *s;
    libtorrent::torrent_handle h;
    libtorrent::file_storage fs;
    //libtorrent::cache_status cache;
    Player *mPlayer;

    int _scene, piece_first;
    double kb_required, kb_skip_global, n_kb;
    bool metadata_ready, dump, skip, aborted;

private slots:
    void start(const QString * const tmp, const QStringList * const file);
    void main_loop();
    void filter_files();
    void stats();
    void error();
};

#endif
