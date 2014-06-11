#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
{
    videoWidget = new QVideoWidget(this);
    player = new QMediaPlayer(this);
    player->setVideoOutput(videoWidget);

    playPauseButton = new QPushButton();
    playPauseButton->setGeometry(2, 1048, 30, 30);
    playPauseButton->setStyleSheet("background: transparent; border: none");
    playPauseButton->setIconSize(QSize(30, 30));
    playPauseButton->setParent(videoWidget);
    playPauseButton->setEnabled(false);
    playPauseButton->hide();

    stopButton = new QPushButton();
    stopButton->setGeometry(34, 1048, 30, 30);
    stopButton->setStyleSheet("background: transparent; border: none");
    stopButton->setIcon(QIcon("://images/player/stop.png"));
    stopButton->setIconSize(QSize(30, 30));
    stopButton->setParent(videoWidget);
    stopButton->setEnabled(false);
    stopButton->hide();

    slider = new QSlider(Qt::Horizontal);
    slider->setGeometry(118, 1063, 1750, 10); // TODO: Ancho
    slider->setStyleSheet("background: white; border: none");
    slider->setTracking(true);
    slider->setMinimum(0);
    slider->setParent(videoWidget);
    slider->setEnabled(false);
    slider->hide();

    volumeButton = new QPushButton();
    volumeButton->setGeometry(76, 1048, 30, 30);
    volumeButton->setStyleSheet("background: transparent; border: none");
    volumeButton->setIcon(QIcon("://images/player/headphone.png"));
    volumeButton->setIconSize(QSize(30, 30));
    volumeButton->setParent(videoWidget);
    volumeButton->setEnabled(false);
    volumeButton->hide();

    volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setGeometry(88, 947, 5, 100);
    volumeSlider->setStyleSheet("background: white; border: none");
    volumeSlider->setRange(0, 100);
    volumeSlider->setParent(videoWidget);
    volumeSlider->setEnabled(false);
    volumeSlider->hide();

    timeLabel = new QLabel();
    timeLabel->setStyleSheet("background: transparent; border: none; color: white");
    timeLabel->setGeometry(1875, 1048, 36, 30);
    timeLabel->setParent(videoWidget);
    timeLabel->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(videoWidget);
    setLayout(layout);

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(statusChange()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChange(qint64)));
    connect(playPauseButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(positionSliderChange()));
    connect(volumeButton, SIGNAL(clicked()), this, SLOT(showHideVolumeSlider()));
    connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVolume(int)));

    // TODO: Falta barra de carga del buffer.
}

VideoPlayer::~VideoPlayer()
{
    player->stop();
    delete player;
    delete videoWidget;
}

void VideoPlayer::open(const QString &file, QObject *obj)
{
    _torrent = obj;
    player->setMedia(QUrl::fromLocalFile(file));
    player->play();
}

void VideoPlayer::playPause()
{
    switch(player->state()) {
    case QMediaPlayer::PlayingState:
        player->pause();
        playPauseButton->setIcon(QIcon("://images/player/play.png"));
        break;
    case QMediaPlayer::PausedState:
        player->setPosition(qint64(slider->value()));
        player->play();
        playPauseButton->setIcon(QIcon("://images/player/pause.png"));
        break;
    case QMediaPlayer::StoppedState:
        player->play();
        playPauseButton->setIcon(QIcon("://images/player/pause.png"));
        break;
    }
}

void VideoPlayer::stop()
{
    player->stop();
}

void VideoPlayer::statusChange()
{
    if (QMediaPlayer::LoadedMedia && QMediaPlayer::PlayingState) {
        playPauseButton->setIcon(QIcon("://images/player/pause.png"));
        playPauseButton->setEnabled(true);
        playPauseButton->show();
        stopButton->setEnabled(true);
        stopButton->show();
        slider->setRange(0, player->duration()); // TODO: Enviar la duracion total desde torrentInfo.
        slider->setEnabled(true);
        slider->show();
        volumeButton->setEnabled(true);
        volumeButton->show();
        volumeSlider->setValue(70);
        volumeSlider->setEnabled(true);
        QString timeStr;
        QString format = "mm:ss";
        //timeStr = duracion_enviada_desde_torrentInfo.toString(format); // TODO: Enviar la duracion total desde torrentInfo.
        timeStr = "35:48";
        timeLabel->setText(timeStr);
        timeLabel->show();
    }
}


void VideoPlayer::positionChange(qint64 position)
{
    slider->setValue(position);
}

void VideoPlayer::positionSliderChange()
{
    // TODO: Saber si la parte a la que se mueve esta descargada, si no lo esta, hacer la siguiente llamada.
    int offset = slider->value()/655;
    QMetaObject::invokeMethod(_torrent, "offsetDownload", Qt::QueuedConnection, Q_ARG(int, offset));

    player->setPosition(qint64(slider->value()));
}

void VideoPlayer::showHideVolumeSlider()
{
    if (volumeSlider->isHidden())
        volumeSlider->show();
    else
        volumeSlider->hide();
}

void VideoPlayer::setVolume(int volume)
{
    player->setVolume(volume);
}

