#include "player.h"

#include <QPalette>

#include <QTimer>
#include <QDebug>

#define qtu( i ) ((i).toUtf8().constData())

Player::Player(const QString *path, QWidget *parent) :
    QWidget(parent),
    kb_writen("0"),
    ui(new Ui::Player)
{
    ui->setupUi(this);

    instance = libvlc_new(0, NULL);
    media = libvlc_media_new_location(instance, qtu(*path));
    mediaplayer = libvlc_media_player_new_from_media(media);

    libvlc_media_release(media);
    libvlc_media_player_set_xwindow(mediaplayer, this->winId());

    QBrush b(QColor(10, 10, 10, 255));
    QPalette p;
    p.setBrush(QPalette::Active, QPalette::Window, b);
    setPalette(p);
    setAutoFillBackground(true);

    //if (libvlc_media_player_will_play(mediaplayer)) {
        libvlc_media_player_play(mediaplayer);  //TODO: CONTROL DE REPRODUCCION
    //  show();
    //} else {
    //  qDebug() << "--> NOT WILL PLAY";
    //}

    stats();

}

Player::~Player()
{
    //TODO: VER SI LO BORRA BIEN

    if (instance) {
        libvlc_release(instance);
        qDebug() << "DELETE PLAYER";
    }
}

void Player::stats()
{
    ui->lb_kb_writen->setText(kb_writen);

    QTimer::singleShot(1000, this, SLOT(stats()));
}

