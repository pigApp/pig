#include "movie.h"

#include <QDebug>

#define qtu( i ) ((i).toUtf8().constData())

Movie::Movie(const QString* const PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    isInitPlayer(false),
    instance(NULL),
    mediaplayer(NULL),
    media(NULL),
    ui(new Ui::Movie)
{
    ui->setupUi(this);
}

Movie::~Movie()
{
    if (instance) {
        libvlc_release(instance);
        qDebug() << "DELETE PLAYER";
    }

    delete ui;
}

void Movie::init_mediaplayer(QString path)
{
    instance = libvlc_new(0, NULL);

    media = libvlc_media_new_path(instance, qtu((*_PIG_PATH)+"/tmp/torrents/movies/"+path));
    //media = libvlc_media_new_location(instance, qtu((*_PIG_PATH)+"/tmp/torrents/movies/"+path)); // Desde URL.

    mediaplayer = libvlc_media_player_new_from_media(media);

    libvlc_media_release(media);
    libvlc_media_player_set_xwindow(mediaplayer, ui->player->winId());

    //if (libvlc_media_player_will_play(mediaplayer)) {
        libvlc_media_player_play(mediaplayer);  //TODO: CONTROL DE REPRODUCCION
    //  show();
    //} else {
    //  qDebug() << "--> NOT WILL PLAY";
    //}
    //ui->l->addWidget(ui->base);
}

void Movie::stats(int bitrate, int peers, const qint64 &kb_writen,
                   const double &kb_required, const double &n_kb)
{
    Q_UNUSED(n_kb);

    ui->lb_bitrate->setText(QString::number(bitrate)+" KB/s");
    ui->lb_peers->setText(QString::number(peers)+" PEERS");

    ui->progressBar->setMaximum(kb_required);
    ui->progressBar->setValue(kb_writen);

    if (instance != 0 && !isInitPlayer) {
        ui->l->removeItem(ui->l_stats);

        ui->progressBar->hide();
        ui->l_stats->setSpacing(0);
        ui->l_controls->addLayout(ui->l_stats);

        ui->l->addWidget(ui->player);
        ui->l->addWidget(ui->controls);

        ui->player->show();
        ui->controls->show();

        isInitPlayer = true;
    }
}
