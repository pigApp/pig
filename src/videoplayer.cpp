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
   
    blackBox = new QWidget();
    blackBox->setGeometry(0, 10, screenWidth, 70);
    blackBox->setStyleSheet("background-color: rgba(0, 0, 0, 20%); border: none;");
    blackBox->setParent(videoWidget);
    blackBox->hide();

    currentTimeLabel = new QLabel();
    currentTimeLabel->setGeometry((screenWidth/2)-160, 20, 160, 50);
    currentTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    currentTimeLabel->setFont(font);
    currentTimeLabel->setParent(videoWidget);
    currentTimeLabel->hide();

    totalTimeLabel = new QLabel();
    totalTimeLabel->setGeometry((screenWidth/2)+10, 20, 160, 50);
    totalTimeLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    totalTimeLabel->setFont(font);
    totalTimeLabel->setParent(videoWidget);
    totalTimeLabel->hide();

    bitRateLabel = new QLabel();
    bitRateLabel->setGeometry(screenWidth-648, 20, 200, 50);
    bitRateLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    bitRateLabel->setFont(font);
    bitRateLabel->setParent(videoWidget);
    bitRateLabel->hide();

    peersLabel = new QLabel();
    peersLabel->setGeometry(screenWidth-460, 20, 230, 50);
    peersLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    peersLabel->setFont(font);
    peersLabel->setParent(videoWidget);
    peersLabel->hide();

    seedsLabel = new QLabel();
    seedsLabel->setGeometry(screenWidth-230, 20, 230, 50);
    seedsLabel->setStyleSheet("background-color: transparent; border: none; color: white");
    seedsLabel->setFont(font);
    seedsLabel->setParent(videoWidget);
    seedsLabel->hide();

    bar = new QProgressBar();
    bar->setGeometry(4, screenHeight-25, screenWidth-8, 1);
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

    // TODO: Barra de carga del buffer.
}

VideoPlayer::~VideoPlayer()
{
    player->stop();
    delete player;
    delete videoWidget;
}

void VideoPlayer::run(QString absoluteFilePath, int totalPieces, int currentPiece)
{
    test = 0;//

    init = true;
    check = true;
    player->setMedia(QUrl::fromLocalFile(absoluteFilePath));
    player->play();
    progress(totalPieces, currentPiece);
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

void VideoPlayer::downloadInfo(int bitRate, int peers, int seeds)
{
    bitRateLabel->setText(QString::number(bitRate)+"Kb/s");
    peersLabel->setText("PEERS "+QString::number(peers));
    seedsLabel->setText("SEEDS "+QString::number(seeds));
}

void VideoPlayer::progress(int totalPieces=0, int currentPiece=0)
{
    int downloadedMsec = (currentPiece*player->duration())/totalPieces;
    int msecForPiece = player->duration()/totalPieces;
    int safePlay = downloadedMsec-((player->duration()/totalPieces)*7);

    bar->setValue(downloadedMsec);

    //qDebug() << "TOTAL_PIECES: " << totalPieces;
    //qDebug() << "CURRENT_PIECE: " << currentPiece;
    //qDebug() << "MSec x PIECE: " << msecForPiece;
    //qDebug() << "SAFE_PLAY_Msec: " << safePlay;

    test = test+3000;
    if (check && player->state() != QMediaPlayer::PausedState) { //&& safePlay !=0 && safePlay >= 0-(msecForPiece*2)) {

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

    if (init && status == QMediaPlayer::BufferedMedia) {
        blackBox->show();
        bar->show();
        currentTimeLabel->show();
        totalTimeLabel->show();
        bitRateLabel->show();
        peersLabel->show();
        seedsLabel->show();
        slider->setEnabled(true);
        slider->show();
        init = false;
    } else if (!init && status == QMediaPlayer::BufferedMedia) {
        player->play();
        check = true;
    } else if (status == QMediaPlayer::InvalidMedia || status == QMediaPlayer::EndOfMedia) {
        check = false;
        //player->pause();
        qDebug() << "FALLO EN Msec: " << slider->value();
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


