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

    Q_INVOKABLE bool availablePiece(int piece);
    Q_INVOKABLE void offsetPiece(int offset);

public slots:
    void download(QString path, QString file, int scenne);

private:
    QObject *_pig;
    QObject *_root;
    session client;
    torrent_handle handle;
    add_torrent_params params;
    torrent_info *info;
    error_code ec;

    QString newFilePath;
    QString newFileName;
    int currentDownloadedPieces;

private slots:
    void neededPiecesToPlay();
};

#endif
