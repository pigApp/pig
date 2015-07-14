#include "player.h"

#include <QPalette>
#include <QDebug>

#define qtu( i ) ((i).toUtf8().constData())

Player::Player(QWidget *parent, QString *file) : QWidget(parent)
{
    instance = libvlc_new(0, NULL);
    media = libvlc_media_new_path(instance, qtu(*file));
    mediaplayer = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);
    libvlc_media_player_set_xwindow(mediaplayer, this->winId());

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);
    setAutoFillBackground(true);
    resize(800, 600);

    //if (libvlc_media_player_will_play(mediaplayer)) {
        libvlc_media_player_play(mediaplayer);
        show();
    //} else {
      //  qDebug() << "--> NOT WILL PLAY";
    //}
}

Player::~Player()
{
    if (instance)
        libvlc_release(instance);
}

