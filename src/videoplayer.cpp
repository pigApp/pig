#include "videoplayer.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>

VideoPlayer::VideoPlayer(QWidget *parent, int screenWidth, int screenHeight) : QWidget(parent)
{    
    videoWidget = new QVideoWidget(this);

    player = new QMediaPlayer(this);
    player->setVideoOutput(videoWidget);
    volume = 50;
    player->setVolume(volume);

    currentTimeLabel = new QLabel();
    currentTimeLabel->setGeometry((screenWidth/2)-40, 0, 55, 30);
    currentTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    currentTimeLabel->setParent(videoWidget);
    currentTimeLabel->hide();

    totalTimeLabel = new QLabel();
    totalTimeLabel->setGeometry((screenWidth/2)+40, 0, 55, 30);
    totalTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    totalTimeLabel->setParent(videoWidget);
    totalTimeLabel->hide();

    slider = new QSlider(Qt::Horizontal);
    slider->setGeometry(4, screenHeight-15, screenWidth-8, 5);
    slider->setStyleSheet("background: white; border: none");
    slider->setMinimum(0);
    slider->setTracking(true);
    slider->setEnabled(false);
    slider->setParent(videoWidget);
    slider->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(videoWidget);
    setLayout(layout);

    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setCurrentTime(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setTotalTime(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPosition(qint64)));
    connect(slider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(statusChange()));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(error(QMediaPlayer::Error)));

    // TODO: Barra de carga del buffer.
}

VideoPlayer::~VideoPlayer()
{
    player->stop();
    delete player;
    delete videoWidget;
}

void VideoPlayer::open(const QString &file, QObject *obj)
{   
    init = true;
    _torrent = obj;

    player->setMedia(QUrl::fromLocalFile(file));
    player->play();
}

void VideoPlayer::playPause()
{
    if (player->state() == QMediaPlayer::PlayingState)
        player->pause();
    else if (player->state() == QMediaPlayer::PausedState)
        player->play();
    else if (player->state() == QMediaPlayer::StoppedState)
        player->play();
}

void VideoPlayer::setCurrentTime(qint64 msecs)
{
    int hours = msecs/(1000*60*60);
    int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    QString formattedTime;
    formattedTime.append(QString( "%1" ).arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

    currentTimeLabel->setText(formattedTime);
}

void VideoPlayer::setTotalTime(qint64 msecs)
{
    slider->setRange(0, msecs);

    int hours = msecs/(1000*60*60);
    int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    QString formattedTime;
    formattedTime.append(QString( "%1" ).arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

    totalTimeLabel->setText(formattedTime);
}

void VideoPlayer::setSliderPosition(qint64 position)
{
    slider->setValue(position);
    qDebug() << "BUFFERED_MEDIA: " << player->bufferStatus();
    qDebug() << "SLIDER POSITION ACTUALIZACION: " << slider->value();
}

void VideoPlayer::sliderPressed()
{
    player->pause();
    qDebug() << "SLIDER PRESSED";
}

void VideoPlayer::sliderMoved(int position)
{
    slider->setValue(position);
    qDebug() << "SLIDER MOVED TO: " << position;
}

void VideoPlayer::sliderReleased()
{
    qDebug() << "SLIDER RELEASED";

    int offset = slider->value()/655;
    bool ret;

    QMetaObject::invokeMethod(_torrent, "availablePiece", Qt::DirectConnection, Q_RETURN_ARG(bool, ret), Q_ARG(int, offset));
    if(!ret) {
        QMetaObject::invokeMethod(_torrent, "offsetPiece", Qt::DirectConnection, Q_ARG(int, offset));
    } else {
        player->setPosition(qint64(slider->value()));
        playPause();
    }
}

void VideoPlayer::update()
{
    player->setPosition(qint64(slider->value()+1000));
    QTimer::singleShot(15000, this, SLOT(playPause()));
}

void VideoPlayer::setPositiveVolume()
{
    if (volume < 100)
        volume = volume+10;
    player->setVolume(volume);
}

void VideoPlayer::setNegativeVolume()
{
    if (volume > 0)
        volume = volume-10;
    player->setVolume(volume);
}

void VideoPlayer::statusChange()
{
    if (init && QMediaPlayer::LoadedMedia && QMediaPlayer::PlayingState) {
        currentTimeLabel->setText("-- : -- : -- | ");
        currentTimeLabel->show();
        totalTimeLabel->setText("-- : -- : --");
        totalTimeLabel->show();
        slider->setEnabled(true);
        slider->show();
        init = false;
    }

    /*
    if (QMediaPlayer::PlayingState) {
        if (tEST != 0 ) {
            qDebug() << "TEST" << ++tEST;
            qDebug() << "LLEGO COMO PLAYING";
            //player->pause();
            playPauseButton->setIcon(QIcon("://images/player/pause.png"));
        }
        tEST = 1;
    } else if (player->state() == QMediaPlayer::PausedState) {
        qDebug() << "LLEGO COMO PAUSED";
        //player->play();
        playPauseButton->setIcon(QIcon("://images/player/play.png"));
    } else if (player->state() == QMediaPlayer::StoppedState) {
        qDebug() << "LLEGO COMO STOPED";
        //player->play();
        //playPauseButton->setIcon(QIcon("://images/player/pause.png"));
    }

    switch(player->state()) {
    case QMediaPlayer::PlayingState:
        player->pause();
        playPauseButton->setIcon(QIcon("://images/player/play.png"));
        break;
    case QMediaPlayer::PausedState:
        player->play();
        playPauseButton->setIcon(QIcon("://images/player/pause.png"));
        break;
    case QMediaPlayer::StoppedState:
        //player->setPosition(qint64(slider->value()));
        break;
    }


    if (QMediaPlayer::LoadedMedia && QMediaPlayer::PlayingState ) { //&& TEST == 0
        playPauseButton->setIcon(QIcon("://images/player/pause.png"));
        playPauseButton->setEnabled(true);
        playPauseButton->show();
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
        totalTimeLabel->setText(timeStr);
        totalTimeLabel->show();

        //TEST = 1;
    }
    */
}

void VideoPlayer::error(QMediaPlayer::Error)
{
    qDebug() << player->QMediaPlayer::error();
}


