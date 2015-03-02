#ifndef TORRENT_H
#define TORRENT_H

#include <libtorrent/session.hpp>

#include <stdlib.h>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent = 0, QObject **root = NULL, const QString *url = NULL);
    ~Torrent();

    int scene;

public slots:
    //bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    //void piece_update(qint64 total_msec, qint64 offset_msec);

private:
    QObject **_root;
    libtorrent::session *s;
    libtorrent::torrent_handle h;

    QString file;
    qint64 piece_first, piece_last, piece_length, piece_offset, n_pieces;
    qint64 piece_offset_global, n_pieces_global;
    qint64 offset, mb_skip_global, n_mb;

    bool metadata_ready, dump, skip, abort;
    int mb_required;

private slots:
    void main_loop();
    void filter_files();
    void progress();

    void call_player();//
};

#endif
