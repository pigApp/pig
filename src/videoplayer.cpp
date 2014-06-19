#include "videoplayer.h"

#include <QTimer>
#include <QDebug>

VideoPlayer::VideoPlayer(QObject *parent, int screenWidth, int screenHeight) : QObject(parent)
{    
    videoWidget = new QVideoWidget();

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

    layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(videoWidget);

    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setCurrentTime(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setTotalTime(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPosition(qint64)));
    connect(slider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(statusChange(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(error(QMediaPlayer::Error)));

    // TODO: Barra de carga del buffer.

    init = true; //
}

VideoPlayer::~VideoPlayer()
{
    player->stop();
    delete player;
    delete videoWidget;
}

void VideoPlayer::playPause()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else if (player->state() == QMediaPlayer::PausedState) {
        player->play();
    }else if (player->state() == QMediaPlayer::StoppedState) {
        player->setPosition(qint64(slider->value()-1000));
        player->play();
    }
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

    int totalMsec = player->duration();
    int offsetMsec = slider->value();
    bool available;

    QMetaObject::invokeMethod(_torrent, "availablePiece", Qt::DirectConnection, Q_RETURN_ARG(bool, available), Q_ARG(int, totalMsec), Q_ARG(int, offsetMsec));
    if(!available) {
        QMetaObject::invokeMethod(_torrent, "offsetPiece", Qt::DirectConnection, Q_ARG(int, totalMsec), Q_ARG(int, offsetMsec));
    } else {
        player->setPosition(qint64(slider->value()));
        playPause();
    }
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

void VideoPlayer::update()
{
    player->setPosition(qint64(slider->value()+1000));
    QTimer::singleShot(2000, this, SLOT(playPause()));
}

void VideoPlayer::progress(int totalPieces, int currentPiece)
{
    qDebug() << "TOTAL: " << totalPieces;
    qDebug() << "CURRENT: " << currentPiece;

    int downloadedMsec = (currentPiece*player->duration())/totalPieces;
    qDebug() << "DONLOADED_Msec: " << downloadedMsec-25000; // TODO: Revisar este calculo.
    if (slider->value() >= downloadedMsec-25000) {
        player->pause();
        qDebug() << "----PAUSED_FROM_PROGRESS";
    } else {
        qDebug() << "++++PLAY FROM PROGRESS";
        if(player->state() == QMediaPlayer::PausedState)
            player->play();
    }
}

void VideoPlayer::statusChange(QMediaPlayer::MediaStatus status)
{
    qDebug() << "===STATUS: " << status;
    if (init && status == QMediaPlayer::BufferedMedia) {
        currentTimeLabel->setText("-- : -- : -- ");
        currentTimeLabel->show();
        totalTimeLabel->setText("-- : -- : --");
        totalTimeLabel->show();
        slider->setEnabled(true);
        slider->show();
        init = false;
    } else if (!init && status == QMediaPlayer::BufferedMedia) {
        player->play();
    } else if (status == QMediaPlayer::InvalidMedia || status == QMediaPlayer::EndOfMedia) {
        player->pause();
        QTimer::singleShot(15000, this, SLOT(playPause()));
    }
}

void VideoPlayer::error(QMediaPlayer::Error)
{
    qDebug() << "--ERROR: " << player->QMediaPlayer::error();
}
