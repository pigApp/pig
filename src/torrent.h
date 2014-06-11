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
    explicit Torrent(QObject *parent, QObject *obj);

    Q_INVOKABLE void offsetDownload(int offset);

public slots:
    void download(QString path, QString file, int scenne);

private:
    session client;
    torrent_handle handle;
    add_torrent_params params;
    torrent_info *info;
    error_code ec;
    QObject *_pig;

    QString pathx;
    QString filex;
    int pieces;

private slots:
    void callPlayer();

};

#endif
