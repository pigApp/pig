#include "movie.h"

#include <QDebug>

#define qtu( i ) ((i).toUtf8().constData())

Movie::Movie(const QString* const PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    isSetTotalTime(false),
    cacheVolume(80),
    instance(NULL),
    mediaplayer(NULL),
    media(NULL),
    ui(new Ui::Movie)
{
    ui->setupUi(this);
}

Movie::~Movie()
{
    if (instance)
        libvlc_release(instance);

    delete ui;
}

void Movie::init_mediaplayer(QString path)
{
    instance = libvlc_new(0, NULL);
    media = libvlc_media_new_path(instance, qtu((*_PIG_PATH)+"/tmp/torrents/movies/"+path));
    mediaplayer = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);
#if defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(mediaplayer, ui->player->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(mediaplayer, ui->player->winId());
#endif
    libvlc_set_fullscreen(mediaplayer, true);//
    libvlc_video_set_aspect_ratio(mediaplayer, "16:9"); //TODO: LLAMAR A FUNCION.
    libvlc_video_set_scale(mediaplayer, 1);

    connect (ui->b_play, SIGNAL(pressed()), this, SLOT(mediaplayer_play()));
    connect (ui->b_mute, SIGNAL(pressed()), this, SLOT(mediaplayer_mute()));
    connect (ui->sl_volume, SIGNAL(sliderMoved(int)), this, SLOT(mediaplayer_set_volume(int)));
    connect (ui->sl_position, SIGNAL(sliderMoved(int)), this, SLOT(mediaplayer_set_position(int)));
    QObject::connect (ui->b_more, &QPushButton::clicked, [&] {
        if (ui->lb_bitrate->isHidden()) {
            ui->lb_bitrate->setHidden(false);
            ui->lb_peers->setHidden(false);
            ui->b_more->setIcon(QIcon(":/icon-less"));
        } else {
            ui->lb_bitrate->setHidden(true);
            ui->lb_peers->setHidden(true);
            ui->b_more->setIcon(QIcon(":/icon-more"));
        }
    });

    ui->l->removeItem(ui->l_vertical_stats);
    ui->lb_bitrate->hide();
    ui->lb_peers->hide();
    ui->pb_minimum_download->hide();
    ui->lb_bitrate->setPalette(ui->p_stats_controls);
    ui->lb_peers->setPalette(ui->p_stats_controls);
    ui->l_vertical_stats->setSpacing(0);
    ui->l_controls->addLayout(ui->l_vertical_stats);
    ui->l->addWidget(ui->player);
    ui->l->addWidget(ui->controls);
    ui->player->show();
    ui->controls->show();

    mediaplayer_play();

    QTimer *t_ui = new QTimer(this);
    connect (t_ui, SIGNAL(timeout()), this, SLOT(mediaplayer_update_ui()));
    t_ui->start(100);

    t_controls = new QTimer(this);
    t_controls->setSingleShot(true);
    QObject::connect (t_controls, &QTimer::timeout, [&] { mediaplayer_controls(false); });
    t_controls->start(5000);
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
            mediaplayer_set_volume(cacheVolume);
            ui->sl_volume->setValue(cacheVolume);
        } else {
            cacheVolume = ui->sl_volume->value(); //TODO: REVISAR.
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

void Movie::set_time(qint64 ms, bool setTotalTime)
{
    const int hh = ms/(1000*60*60);
    const int mm = (ms-(hh*1000*60*60))/(1000*60);
    const int ss = (ms-(mm*1000*60)-(hh*1000*60*60))/1000;

    QString fTime;
    fTime.append(QString("%1").arg(hh, 2, 10, QLatin1Char('0')) + ":" +
                 QString("%1").arg(mm, 2, 10, QLatin1Char('0')) + ":" +
                 QString("%1").arg(ss, 2, 10, QLatin1Char('0')));

    if (setTotalTime) {
        //ui->sl_position->setMaximum(ms); //TODO: REVISAR SI VA.
        //ui->sl_position->setRange(0, ms);
        ui->lb_total_time->setText(fTime);
        isSetTotalTime = true;
    } else {
        ui->lb_current_time->setText(fTime);
    }
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

void Movie::mediaplayer_controls(bool show)
{
    if (show) {
        libvlc_video_set_scale(mediaplayer, 1);
        ui->controls->show();
    } else {
        libvlc_video_set_scale(mediaplayer, 0);
        ui->controls->hide();
    }
}

void Movie::mediaplayer_update_ui()
{
    if (!mediaplayer)
        return;

    float pos = libvlc_media_player_get_position(mediaplayer);
    ui->sl_position->setValue((int)(pos * 1000.0));

    if ((libvlc_media_player_get_length(mediaplayer) != -1) && !isSetTotalTime)
        set_time(libvlc_media_player_get_length(mediaplayer), true);
    else
        set_time(libvlc_media_player_get_time(mediaplayer));

    if (((ui->player->mapFromGlobal(QCursor::pos()).y()) > 1030) &&
         (ui->controls->isHidden())) //TODO: 1030 porcentaje.
        mediaplayer_controls();
    else if (((ui->player->mapFromGlobal(QCursor::pos()).y()) < 1030) &&
             (!ui->controls->isHidden()) && (!t_controls->isActive())) //TODO: 1030 porcentaje.
        mediaplayer_controls(false);

    if (libvlc_media_player_get_state(mediaplayer) == libvlc_Ended)
        mediaplayer_stop();
}

void Movie::stats(int bitrate, int peers, const qint64 &kb_writen,
                  const double &kb_required, const double &n_kb)
{
    Q_UNUSED(n_kb);

    ui->lb_bitrate->setText(QString::number(bitrate)+" KBPS");
    ui->lb_peers->setText(QString::number(peers)+" PEERS");

    ui->pb_minimum_download->setMaximum(kb_required);
    ui->pb_minimum_download->setValue(kb_writen);
}


//media = libvlc_media_new_location(instance, qtu((*_PIG_PATH)+"/tmp/torrents/movies/"+path)); // Desde URL.

//if (libvlc_media_player_will_play(mediaplayer)) {
    //libvlc_media_player_play(mediaplayer);  //TODO: CONTROL DE REPRODUCCION
//  show();
//} else {
//  qDebug() << "--> NOT WILL PLAY";
//}
//ui->l->addWidget(ui->base);
