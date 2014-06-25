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

    Q_INVOKABLE bool isAvailable(int totalMsec, int offsetMsec, int availablePiece);
    Q_INVOKABLE void offsetPiece(int totalMsec, int offsetMsec);

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
    bool toBox;
    int offset;
    int neededPices;
    int availablePiece;
    int downloadOffsetPieces;
    int neededBytes;

private slots:
    void metadataReady();
    void minimumPiecesReady();
    void progress();
    void downloadInfo();
};

#endif
