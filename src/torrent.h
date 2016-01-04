#ifndef TORRENT_H
#define TORRENT_H

#pragma GCC system_header

#include "movie.h"

#include <libtorrent/session.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(const QString* const PIG_PATH, const QString *host, const QString *url,
                     const QString *pkg, int scene, Movie **movie, QObject *parent = 0);

    ~Torrent();

signals:
    void sendFile(QString path);
    void sendStats(int bitrate, int peers, const qint64 &kb_writen,
                   const double &kb_required, const double &n_kb);

public slots:
    //bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    //void piece_update(qint64 total_msec, qint64 offset_msec);

private:
    const QString* const _PIG_PATH;
    const QString *_host;
    const QString *_url;
    const QString *_pkg;
    int _scene;
    Movie **_player;

    libtorrent::session s;
    libtorrent::torrent_handle h;
    libtorrent::file_storage fs;

    int piece_first;
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
