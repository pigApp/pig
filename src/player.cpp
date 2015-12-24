#include "player.h"

#include <QTimer>
#include <QDebug>

#define qtu( i ) ((i).toUtf8().constData())

Player::Player(const QString *path, QWidget *parent) :
    QWidget(parent),
    m_bitrate("0"),
    m_peers("0"),
    m_kb_required(0),
    m_n_kb(0),
    m_kb_writen(0),
    ui(new Ui::Player)
{
    ui->setupUi(this);

    instance = libvlc_new(0, NULL);
    media = libvlc_media_new_location(instance, qtu(*path));
    mediaplayer = libvlc_media_player_new_from_media(media);

    libvlc_media_release(media);
    libvlc_media_player_set_xwindow(mediaplayer, this->winId());

    setPalette(ui->p_player);
    setAutoFillBackground(true);

    //if (libvlc_media_player_will_play(mediaplayer)) {
        //libvlc_media_player_play(mediaplayer);  //TODO: CONTROL DE REPRODUCCION
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

    delete ui;
}

void Player::stats()
{
    ui->lb_bitrate->setText(m_bitrate);
    ui->lb_peers->setText(m_peers);

    qDebug() << "KB_REQUIRED: " << m_kb_required;
    qDebug() << "N_KB: " << m_n_kb;
    qDebug() << "KB_WRITEN: " << m_kb_writen;

    QTimer::singleShot(1000, this, SLOT(stats()));
}

