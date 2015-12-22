#ifndef PLAYER_H
#define PLAYER_H

#include "ui.h"

#include <vlc/vlc.h> //<pig/vlc/vlc.h>

#include <QWidget>

#include <QObject>

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(const QString *path = NULL, QWidget *parent = 0);
    ~Player();

    libvlc_media_player_t *mediaplayer;

    QString kb_writen;

private:
    Ui::Player *ui;

    libvlc_instance_t *instance;
    libvlc_media_t *media;
    //libvlc_media_stats_t *stats;

private slots:
    void stats();
};

#endif
