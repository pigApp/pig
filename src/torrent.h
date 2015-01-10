#ifndef TORRENT_H
#define TORRENT_H

#include <libtorrent/session.hpp>

#include <stdlib.h>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent=0, const QString *url=NULL);
    ~Torrent();

    QObject **_root;

    int scene;

public slots:
    //bool piece_is_available(qint64 total_msec, qint64 offset_msec);
    //void piece_update(qint64 total_msec, qint64 offset_msec);

private:
    libtorrent::session *s;
    libtorrent::torrent_handle h;

    QString file;
    qint64 pieceFirst, pieceLast, pieceTotal, pieceLength, pieceOffset;
    qint64 pieceTotalGlobal, pieceOffsetGlobal;
    qint64 total, offset, skipGlobal;

    bool dump, aborted, skip;
    int required;

private slots:
    void get();
    void filter();
    void progress();
};

#endif
