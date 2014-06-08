#ifndef TORRENT_H
#define TORRENT_H

#include <stdlib.h>
#include <iostream>

#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/torrent_info.hpp>

using namespace libtorrent;

class Torrent
{

public:
    Torrent();
    ~Torrent();

    void download();//(QString tmpPath, QString fileTorrent);

private:
    session s;
    error_code ec;
    add_torrent_params params;
    torrent_handle handle;
};

#endif
