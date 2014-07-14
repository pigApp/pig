#ifndef TORRENT_H
#define TORRENT_H

#include <libtorrent/session.hpp>

#include <stdlib.h>
#include <iostream>

#include <QObject>

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent=0);

    Q_INVOKABLE bool isAvailable(int total_msec, int offset_msec, int availablePiece);
    Q_INVOKABLE void offsetPiece(int total_msec, int offset_msec);

    QObject *_pig;
    QObject *_player;
    QObject *_root;

    int scenne;

public slots:
    void doRun(QString mangnetUrl);

private:
    libtorrent::session client;
    libtorrent::torrent_handle handle;
    libtorrent::add_torrent_params params;
    libtorrent::error_code ec;

    bool remap;
    bool widget;
    int offset;
    int neededPices;
    int availablePiece;
    int downloadOffsetPieces;
    int needed_kb;

    bool skip;

private slots:
    void metadataReady();
    void minimumPiecesReady();
    void progress();
    void downloadInfo();
};
#endif
