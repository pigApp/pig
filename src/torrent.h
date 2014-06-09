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
    explicit Torrent(QObject *parent = 0);

public slots:
    void download(QString path, QString file);

signals:
    void readyToRead();

private:
    session client;
    torrent_handle handle;
    add_torrent_params params;
    error_code ec;

private slots:
    void progressX();

};

#endif
