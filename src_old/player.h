#ifndef PLAYER_H
#define PLAYER_H

#include <vlc/vlc.h> //<pig/vlc/vlc.h>

#include <QObject>
#include <QWidget>

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0, QString *file = NULL);
    ~Player();

    libvlc_media_player_t *mediaplayer;

private:
    libvlc_instance_t *instance;
    libvlc_media_t *media;
    libvlc_media_stats_t *stats;
};

#endif
