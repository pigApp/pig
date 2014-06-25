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

    QFont font( "helvetica", 30);
   
    box = new QWidget();
    box->setGeometry(0, 10, screenWidth, 70);
    box->setStyleSheet("background-color: rgba(0, 0, 0, 20%); border: none;");
    box->setParent(videoWidget);
    box->hide();

    totalTimeLabel = new QLabel();
    totalTimeLabel->setGeometry((screenWidth/2)+10, 20, 160, 50);
    totalTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    totalTimeLabel->setFont(font);
    totalTimeLabel->setParent(videoWidget);
    totalTimeLabel->hide();

    currentTimeLabel = new QLabel();
    currentTimeLabel->setGeometry((screenWidth/2)-160, 20, 160, 50);
    currentTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    currentTimeLabel->setFont(font);
    currentTimeLabel->setParent(videoWidget);
    currentTimeLabel->hide();

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
    pausedForUser = false;
    control = true;
    player->setMedia(QUrl::fromLocalFile(absoluteFilePath));
    player->play();
}

void VideoPlayer::playPauseForUser()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        pausedForUser = true;
    } else if (player->state() == QMediaPlayer::PausedState) {
        player->play();
        pausedForUser = false;
    }
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

void VideoPlayer::progress(int totalPieces=0, int availablePiece=0)
{
    int pieceMsec = player->duration()/totalPieces;
    //qDebug() << "PIECE_Msec: " << pieceMsec;

    int downloadedMsec = ((availablePiece*player->duration())/totalPieces)+(pieceMsec*5);
    int availableMsec = ((availablePiece*player->duration())/totalPieces)-(pieceMsec*5); 

    bar->setValue(downloadedMsec);

    if (control && !pausedForUser) {
        if (slider->value() < availableMsec) {
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

    qDebug() << "AVAILABLE_Msec: " << availableMsec;
    qDebug() << "CURRENT_Msec: " << player->position();
}

void VideoPlayer::downloadInfo(int bitRate, int peers)
{
    bitRateLabel->setText(QString::number(bitRate)+"Kb/s");
    peersLabel->setText("PEERS "+QString::number(peers));
}

void VideoPlayer::update()
{
    player->setPosition(qint64(slider->value()+1000));
    QTimer::singleShot(2000, this, SLOT(playPause()));
}

void VideoPlayer::error(QMediaPlayer::Error)
{
    qDebug() << "--ERROR: " << player->QMediaPlayer::error();
}

void VideoPlayer::statusChange(QMediaPlayer::MediaStatus status)
{
    qDebug() << "===STATUS: " << status;

    if (status == QMediaPlayer::BufferedMedia) {
        if (!slider->isEnabled()) {
            box->show();
            totalTimeLabel->show();
            currentTimeLabel->show();
            bitRateLabel->show();
            peersLabel->show();
            bar->show();
            slider->setEnabled(true);
            slider->show();
        } else {
            player->play();
            control = true;
        }
    } else if (status == QMediaPlayer::InvalidMedia || status == QMediaPlayer::EndOfMedia) {
        control = false;
        QTimer::singleShot(40000, this, SLOT(playPause()));
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
    player->pause();
}

void VideoPlayer::sliderMoved(int position)
{
    slider->setValue(position);
}

void VideoPlayer::sliderReleased()
{
    int totalMsec = player->duration();
    int offsetMsec = slider->value();
    bool available;

    QMetaObject::invokeMethod(_torrent, "isAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, available), Q_ARG(int, totalMsec), Q_ARG(int, offsetMsec), Q_ARG(int, 0));
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
        volume = volume+5;
    player->setVolume(volume);
}

void VideoPlayer::setNegativeVolume()
{
    if (volume > 0)
        volume = volume-5;
    player->setVolume(volume);
}







/*
int downloadedMsec = (currentPiece*player->duration())/totalPieces;
int msecForPiece = player->duration()/totalPieces;
int safePlay = downloadedMsec-((player->duration()/totalPieces)*7);

//qDebug() << "TOTAL_PIECES: " << totalPieces;
//qDebug() << "CURRENT_PIECE: " << currentPiece;
//qDebug() << "MSec x PIECE: " << msecForPiece;
//qDebug() << "SAFE_PLAY_Msec: " << safePlay;

test = test+3000;
if (control && player->state() != QMediaPlayer::PausedState) { //&& safePlay !=0 && safePlay >= 0-(msecForPiece*2)) {

    //if (slider->value() < safePlay) {
    if (slider->value() < test) {
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
qDebug() << "TEST   Msec: " << test;//
qDebug() << "SLIDER Msec: " << slider->value();
*/
