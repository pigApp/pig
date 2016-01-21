#include "movie.h"

#include <QTimer>
#include <QDebug>

#define qtu( i ) ((i).toUtf8().constData())

Movie::Movie(const QString* const PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    isInitMediaplayer(false),
    lastVol(80),
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

    connect(ui->b_play, SIGNAL(pressed()), this, SLOT(mediaplayer_play()));
    connect(ui->b_mute, SIGNAL(pressed()), this, SLOT(mediaplayer_mute()));
    connect(ui->sl_volume, SIGNAL(sliderMoved(int)), this, SLOT(mediaplayer_set_volume(int)));
    connect(ui->sl_position, SIGNAL(sliderMoved(int)), this, SLOT(mediaplayer_set_position(int)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(mediaplayer_update_ui()));
    timer->start(100);

    mediaplayer_play();

    //if (libvlc_media_player_will_play(mediaplayer)) {
        libvlc_media_player_play(mediaplayer);  //TODO: CONTROL DE REPRODUCCION
    //  show();
    //} else {
    //  qDebug() << "--> NOT WILL PLAY";
    //}
    //ui->l->addWidget(ui->base);
}

void Movie::mediaplayer_play()
{
    if (mediaplayer) {
        if (libvlc_media_player_is_playing(mediaplayer)) {
            libvlc_media_player_pause(mediaplayer);
            ui->b_play->setIcon(QIcon(":/icon-play"));
        } else {
            libvlc_media_player_play(mediaplayer);
            ui->b_play->setIcon(QIcon(":/icon-pause"));
        }
    }
}

void Movie::mediaplayer_mute()
{
    if(mediaplayer) {
        if(ui->sl_volume->value() == 0) {
            mediaplayer_set_volume(lastVol);
            ui->sl_volume->setValue(lastVol);
        } else {
            lastVol = ui->sl_volume->value(); //TODO: REVISAR.
            mediaplayer_set_volume(0);
            ui->sl_volume->setValue(0);
        }
    }
}

int Movie::mediaplayer_set_volume(int vol)
{
    if (mediaplayer) {
        if (vol == 0)
            ui->b_mute->setIcon(QIcon(":/icon-mute-off"));
        else
            ui->b_mute->setIcon(QIcon(":/icon-mute"));

        return libvlc_audio_set_volume(mediaplayer, vol);
    }

    return 0;
}

void Movie::mediaplayer_set_position(int pos)
{
    if (mediaplayer)
        libvlc_media_player_set_position(mediaplayer, (float)pos/1000.0);
}

void Movie::mediaplayer_stop()
{
    if (mediaplayer) {
        libvlc_media_player_stop(mediaplayer);
        //libvlc_media_player_release(mediaplayer);
        //mediaplayer = NULL;

        ui->sl_position->setValue(0);
        ui->b_play->setIcon(QIcon(":/icon-play"));
    }
}

void Movie::mediaplayer_update_ui()
{
    if (!mediaplayer)
        return;

    //TODO: Revisar 'A ERROR OCURRED'.
    //qDebug() << libvlc_media_player_get_time(vlcPlayer)/1000;

    float pos = libvlc_media_player_get_position(mediaplayer);
    ui->sl_position->setValue((int)(pos*1000.0));

    if (libvlc_media_player_get_state(mediaplayer) == libvlc_Ended)
        mediaplayer_stop();
}

void Movie::stats(int bitrate, int peers, const qint64 &kb_writen,
                  const double &kb_required, const double &n_kb)
{
    Q_UNUSED(n_kb);

    ui->lb_bitrate->setText(QString::number(bitrate)+" KB/s");
    ui->lb_peers->setText(QString::number(peers)+" PEERS");

    ui->progressBar->setMaximum(kb_required);
    ui->progressBar->setValue(kb_writen);

    if (instance != 0 && !isInitMediaplayer) {
        ui->l->removeItem(ui->l_stats);

        ui->progressBar->hide();
        ui->l_stats->setSpacing(0);
        ui->l_controls->addLayout(ui->l_stats);

        ui->l->addWidget(ui->player);
        ui->l->addWidget(ui->controls);

        ui->player->show();
        ui->controls->show();

        isInitMediaplayer = true;
    }
}
