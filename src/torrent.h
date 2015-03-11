#ifndef TORRENT_H
#define TORRENT_H

#include "tcpSocket.h"//

//#include <libtorrent/torrent.hpp>//
#include <libtorrent/session.hpp>

#include <stdlib.h>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent = 0, QObject **root = NULL, const int * const scene = NULL);
    ~Torrent();

public slots:
    void get(const QString * const host, const QString * const url);

    //bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    //void piece_update(qint64 total_msec, qint64 offset_msec);

private:
    QObject **_root;
    TcpSocket *mSocket;//
    //libtorrent::torrent::request_time_critical_pieces();
    libtorrent::session *s;
    libtorrent::torrent_handle h;
    libtorrent::file_storage fs;

    int _scene, piece_first, mb_required, mb_skip_global, n_mb;
    bool metadata_ready, dump, skip, abort;

private slots:
    void start(const QString * const tmp, const QStringList * const file);
    void main_loop();
    void filter_files();
    void ret();
    void information();
};

#endif
