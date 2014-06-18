#ifndef TORRENT_H
#define TORRENT_H

#include <QObject>

#include <stdlib.h>
#include <iostream>

#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/torrent_info.hpp>

using namespace libtorrent;

class Torrent : public QObject
{
    Q_OBJECT

public:
    explicit Torrent(QObject *parent, QObject *obj, QObject *obj2);

    Q_INVOKABLE bool availablePiece(int totalMsec, int offsetMsec);
    Q_INVOKABLE void offsetPiece(int totalMsec, int offsetMsec);

    int scenne;

public slots:
    void download(QString mangnetUrl);

private:
    QObject *_pig;
    QObject *_root;

    session client;
    torrent_handle handle;
    add_torrent_params params;
    error_code ec;

    bool remap;
    int currentDownloadedPieces;

private slots:
    void controlPieces();
    void progress();
};

#endif
