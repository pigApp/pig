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
    ~Torrent();

    Q_INVOKABLE bool piece_is_available(int total_msec, int offset_msec, int piece);
    Q_INVOKABLE void piece_offset(int total_msec, int offset_msec);

    QObject *_pig;
    QObject *_player;
    QObject *_root;

    int scenne;

public slots:
    void doConnect(QString mangnetUrl);
    void stop();

private:
    libtorrent::session *client;
    libtorrent::torrent_handle handle;
    libtorrent::add_torrent_params params;
    libtorrent::error_code ec;

    bool abort;
    bool remap;
    bool skip;
    bool toWidget;
    int piece_kb;
    int required_kb;
    int requiredPieces;
    int availablePiece;
    int downloadedPieces;
    int offset;

private slots:
    void ready_to_download();
    void minimum_pieces_required();
    void progress();
    void download_Information();
};

#endif
