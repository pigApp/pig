#ifndef TORRENT_H
#define TORRENT_H

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
    explicit Torrent(const QString* const PIG_PATH, const QString *host, const QString *url,
                     const QString *pkg, int scene, Player **player, QObject *parent = 0);

    ~Torrent();

public slots:
    //bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    //void piece_update(qint64 total_msec, qint64 offset_msec);

private:
    const QString* const _PIG_PATH;
    const QString *_host;
    const QString *_url;
    const QString *_pkg;
    Player **_player;

    //libtorrent::torrent::request_time_critical_pieces();

    libtorrent::session *s;
    libtorrent::torrent_handle h;
    libtorrent::file_storage fs;
    libtorrent::cache_status cache;

    int _scene, piece_first;
    double kb_required, kb_skip_global, n_kb;
    bool hasMetadata, isDump, isSkip, isAborted;

private slots:
    void init(int ID, QString path);
    void main_loop();
    void filter_files();
    void stats();
    void error(QString error);
};

#endif
