#include "videoplayer.h"

#include <QTimer>
#include <QFont>
#include <QDebug>

VideoPlayer::VideoPlayer(QObject *parent, int screenWidth, int screenHeight) : QObject(parent)
{    
    videoWidget = new QVideoWidget();
    player = new QMediaPlayer(this);
    player->setVideoOutput(videoWidget);
    volume = 70;
    player->setVolume(volume);

    QFont font(":/images/font/pig.ttf", 30); // TODO: Instalar msFonts
   
    box = new QWidget();
    box->setGeometry(0, 10, screenWidth, 70);
    box->setStyleSheet("background-color: rgba(0, 0, 0, 20%); border: none;");
    box->setParent(videoWidget);
    box->hide();

    volumeIcon = new QLabel();
    icon.load((":/images/player/volume.png"));
    volumeIcon->setPixmap(icon);
    volumeIcon->setGeometry(30, 30, 30, 30);
    volumeIcon->setStyleSheet("background-color: transparent; border: none");
    volumeIcon->setParent(videoWidget);
    volumeIcon->hide();

    volumeLabel = new QLabel();
    volumeLabel->setGeometry(70, 20, 100, 50);
    volumeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    volumeLabel->setFont(font);
    volumeLabel->setParent(videoWidget);
    volumeLabel->hide();
    volumeLabel->setText(QString::number(volume));

    pauseLabel = new QLabel();
    pauseLabel->setGeometry(170, 15, 35, 50);
    pauseLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    pauseLabel->setFont(font);
    pauseLabel->setParent(videoWidget);
    pauseLabel->hide();
    pauseLabel->setText("||");

    currentTimeLabel = new QLabel();
    currentTimeLabel->setGeometry((screenWidth/2)-190, 20, 200, 50);
    currentTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    currentTimeLabel->setFont(font);
    currentTimeLabel->setParent(videoWidget);
    currentTimeLabel->hide();

    totalTimeLabel = new QLabel();
    totalTimeLabel->setGeometry((screenWidth/2)+15, 20, 200, 50);
    totalTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    totalTimeLabel->setFont(font);
    totalTimeLabel->setParent(videoWidget);
    totalTimeLabel->hide();

    bitRateLabel = new QLabel();
    bitRateLabel->setGeometry(screenWidth-430, 20, 200, 50);
    bitRateLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    bitRateLabel->setFont(font);
    bitRateLabel->setParent(videoWidget);
    bitRateLabel->hide();

    peersLabel = new QLabel();
    peersLabel->setGeometry(screenWidth-240, 20, 230, 50); //460
    peersLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    peersLabel->setFont(font);
    peersLabel->setParent(videoWidget);
    peersLabel->hide();

    bar = new QProgressBar();
    bar->setGeometry(4, screenHeight-25, screenWidth-8, 3);
    bar->setTextVisible(false);
    bar->setMinimum(0);
    bar->setParent(videoWidget);
    bar->hide();
    
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
}

VideoPlayer::~VideoPlayer()
{
    player->stop();
    delete player;
    delete videoWidget;
}

void VideoPlayer::doRun(QString absoluteFilePath)
{
    skip = false;
    skip_key_value = 0;
    paused = false;
    player->setMedia(QUrl::fromLocalFile(absoluteFilePath));
    player->play();
}

void VideoPlayer::standBy()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else if (player->state() == QMediaPlayer::PausedState) {
        player->setPosition(skip_player_msec);
        player->play();
    } else if (player->state() == QMediaPlayer::StoppedState) {
        player->setPosition(qint64(slider->value()-1000));
        player->play();
    }
}

void VideoPlayer::playPause()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        paused = true;
        pauseLabel->show();
    } else if (player->state() == QMediaPlayer::PausedState && !skip) {
        player->play();
        paused = false;
        pauseLabel->hide();
    }
}

void VideoPlayer::downloadInfo(int bitRate, int peers)
{
    bitRateLabel->setText(QString::number(bitRate)+"Kb/s");
    peersLabel->setText("PEERS "+QString::number(peers));
}

void VideoPlayer::progress(int piece_kb=0, int total_kb=0, int downloaded_kb=0)
{
    if (!skip) {
        int downloaded_msec = ((downloaded_kb-(piece_kb*3))*player->duration())/total_kb; //TODO: No usar cantidad de piezas para resta, usar milisegundos. Sino varia segun el tamaño de pieza
        bar->setValue(downloaded_msec);
        qDebug() << "DOWNLOADED_MSEC: " << downloaded_msec;

        int readyToRead_msec = ((downloaded_kb-(piece_kb*8))*player->duration())/total_kb;
        if (!paused) {
            if (slider->value() < readyToRead_msec) {
                if(player->state() == QMediaPlayer::PausedState) {
                    qDebug() << "++++PLAY FROM PROGRESS";
                    player->play();
                }
            } else {
                if(player->state() == QMediaPlayer::PlayingState) {
                    qDebug() << "----PAUSED_FROM_PROGRESS";
                    player->pause();
                }
            }
        }

        qDebug() << "CURRENT_MSEC: " << slider->value();
        qDebug() << "READY___MSEC: " << readyToRead_msec;
    }
}

void VideoPlayer::update(int total_pieces, int currentPiece)
{
    skip_player_msec = ((currentPiece*player->duration())/total_pieces)+5000;

    QTimer::singleShot(5000, this, SLOT(standBy()));

    //skip = false;
    qDebug() << "PLAYER_SKIP_TO: " << skip_player_msec;
}

void VideoPlayer::statusChange(QMediaPlayer::MediaStatus status)
{
    qDebug() << "===STATUS: " << status;

    if (status == QMediaPlayer::BufferedMedia) {
        if (!slider->isEnabled()) {
            box->show();
            volumeIcon->show();
            volumeLabel->show();
            currentTimeLabel->show();
            totalTimeLabel->show();
            bitRateLabel->show();
            peersLabel->show();
            bar->show();
            slider->setEnabled(true);
            slider->show();
        } else if (!skip){
            player->play();
        }
    } else if (status == QMediaPlayer::InvalidMedia || status == QMediaPlayer::EndOfMedia) {
        skip = false;
        QTimer::singleShot(40000, this, SLOT(standBy()));
    }
}

void VideoPlayer::error(QMediaPlayer::Error)
{
    qDebug() << "--ERROR: " << player->QMediaPlayer::error();
}

void VideoPlayer::setVolume(int volume_key_value)
{
    if ((volume > 0 && volume < 100) || (volume == 0 && volume_key_value > 0) || (volume == 100 && volume_key_value < 0)) {
        volume = volume+volume_key_value;
        player->setVolume(volume);
        volumeLabel->setText(QString::number(volume));
        if (volume > 0) {
            icon.load(":/images/player/volume.png");
            volumeIcon->setPixmap(icon);
        } else {
            icon.load(":/images/player/volumeOff.png");
            volumeIcon->setPixmap(icon);
        }
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
    bar->setMaximum(msecs);
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
}

void VideoPlayer::sliderPressed()
{
    skip = true;
    player->pause();
}

void VideoPlayer::sliderMoved(int position)
{
    slider->setValue(position);
}

void VideoPlayer::sliderReleased()
{
    int total_msec = player->duration();
    int offset_msec = slider->value()+skip_key_value;
    bool available;

    QMetaObject::invokeMethod(_torrent, "isAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, available), Q_ARG(int, total_msec), Q_ARG(int, offset_msec), Q_ARG(int, 0));
    if(available) {
        if (skip_key_value != 0) {
            player->setPosition(qint64(slider->value()+skip_key_value));
            skip_key_value = 0;
        } else {
            skip = false;
            player->setPosition(qint64(slider->value())); // TODO: Al mover el slider pierden el foco las teclas de volume y skip.
            player->play();
        }
    } else if (!available && skip_key_value == 0) {
        bar->setValue(qint64(slider->value()));
        QMetaObject::invokeMethod(_torrent, "offsetPiece", Qt::DirectConnection, Q_ARG(int, total_msec), Q_ARG(int, offset_msec));
    } else if (!available && skip_key_value != 0) {
        skip_key_value = 0;
    }
}
