#include "player.h"

#include <QDebug>

#define qtu( i ) ((i).toUtf8().constData())

Player::Player(const QString* const PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    instance(NULL),
    mediaplayer(NULL),
    media(NULL),
    ui(new Ui::Player)
{
    ui->setupUi(this);
}

Player::~Player()
{
    if (instance) {
        libvlc_release(instance);
        qDebug() << "DELETE PLAYER";
    }

    delete ui;
}

void Player::init_mediaplayer(QString path)
{
    instance = libvlc_new(0, NULL);

    media = libvlc_media_new_path(instance, qtu((*_PIG_PATH)+"/tmp/torrents/movies/"+path));
    //media = libvlc_media_new_location(instance, qtu((*_PIG_PATH)+"/tmp/torrents/movies/"+path)); // Desde URL.

    mediaplayer = libvlc_media_player_new_from_media(media);

    libvlc_media_release(media);
    libvlc_media_player_set_xwindow(mediaplayer, this->winId());

    //if (libvlc_media_player_will_play(mediaplayer)) {
        libvlc_media_player_play(mediaplayer);  //TODO: CONTROL DE REPRODUCCION
    //  show();
    //} else {
    //  qDebug() << "--> NOT WILL PLAY";
    //}
}

void Player::stats(int bitrate, int peers, const qint64 &kb_writen,
                   const double &kb_required, const double &n_kb)
{
    ui->lb_bitrate->setText(QString::number(bitrate)+" KB/s");
    ui->lb_peers->setText(QString::number(peers)+" PEERS");

    ui->progressBar->setMaximum(kb_required);

    ui->progressBar->setValue(kb_writen);

    qDebug() << "KB_WRITEN: " << kb_writen;
    qDebug() << "KB_REQUIRED: " << kb_required;
    qDebug() << "N_KB: " << n_kb;
}
