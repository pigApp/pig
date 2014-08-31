#include "videoplayer.h"

#include <QScreen>
#include <QTimer>
#include <QFont>
#include <QDebug>

VideoPlayer::VideoPlayer(QVideoWidget *parent) : QVideoWidget(parent)
{    
    //QScreen *screen;
    //int screenWidth =  screen->geometry().width();
    //int screenHeight = screen->geometry().height(); // TODO: Obtener width y height.

    screenWidth = 1920;
    screenHeight = 1080;

    player = new QMediaPlayer();
    player->setVideoOutput(this);
    volume = 70;
    player->setVolume(volume);

    QFont font(":/images/font/pig.ttf", 30);

    box = new QWidget();
    box->setGeometry(0, 10, screenWidth, 70);
    box->setStyleSheet("background: black; border: none;");
    box->setParent(this);
    box->hide();

    volumeIcon = new QLabel();
    volumePixmap.load((":/images/player/volume.png"));
    volumeIcon->setPixmap(volumePixmap);
    volumeIcon->setGeometry(30, 30, 30, 30);
    volumeIcon->setStyleSheet("background: black; border: none;");
    volumeIcon->setParent(this);
    volumeIcon->hide();

    volumeLabel = new QLabel();
    volumeLabel->setGeometry(70, 20, 100, 50);
    volumeLabel->setStyleSheet("background: black; border: none; color: white;");
    volumeLabel->setFont(font);
    volumeLabel->setParent(this);
    volumeLabel->hide();
    volumeLabel->setText(QString::number(volume));

    pauseLabel = new QLabel();
    pauseLabel->setGeometry(170, 15, 35, 50);
    pauseLabel->setStyleSheet("background: black; border: none; color: white;");
    pauseLabel->setFont(font);
    pauseLabel->setParent(this);
    pauseLabel->hide();
    pauseLabel->setText("||");

    QFont bufferFont(":/images/font/pig.ttf", 15);
    bufferLabel = new QLabel();
    bufferLabel->setGeometry(170, 28, 120, 35);
    bufferLabel->setStyleSheet("background: black; border: none; color: #181818;");
    bufferLabel->setFont(bufferFont);
    bufferLabel->setParent(this);
    bufferLabel->hide();
    bufferLabel->setText("BUFFERING");
    
    witnessBufferLabel = new QLabel();
    witnessBufferLabel->setGeometry(292, 28, 15, 35);
    witnessBufferLabel->setStyleSheet("background: black; border: none; color: white;");
    witnessBufferLabel->setFont(bufferFont);
    witnessBufferLabel->setParent(this);
    witnessBufferLabel->hide();
    witnessBufferLabel->setText("•");

    startLoopTimeLabel = new QLabel();
    startLoopTimeLabel->setGeometry(170, 20, 200, 50);
    startLoopTimeLabel->setStyleSheet("background: black; border: none; color: #181818;");
    startLoopTimeLabel->setFont(font);
    startLoopTimeLabel->setParent(this);
    startLoopTimeLabel->hide();

    stopLoopTimeLabel = new QLabel();
    stopLoopTimeLabel->setGeometry(370, 20, 200, 50);
    stopLoopTimeLabel->setStyleSheet("background: black; border: none; color: #181818;");
    stopLoopTimeLabel->setFont(font);
    stopLoopTimeLabel->setParent(this);
    stopLoopTimeLabel->hide();

    barOffset = new QProgressBar();
    barOffset->setGeometry(170, 45, screenWidth/13, 3);
    barOffset->setStyleSheet (
                   "QProgressBar { background: #181818; border: none; }"
                   "QProgressBar::chunk { background: white; }"
               );
    barOffset->setTextVisible(false);
    barOffset->setMinimum(0);
    barOffset->setMaximum(15);
    barOffset->setParent(this);
    barOffset->hide();

    currentTimeLabel = new QLabel();
    currentTimeLabel->setGeometry((screenWidth/2)-190, 20, 200, 50);
    currentTimeLabel->setStyleSheet("background: black; border: none; color: white;");
    currentTimeLabel->setFont(font);
    currentTimeLabel->setParent(this);
    currentTimeLabel->hide();

    totalTimeLabel = new QLabel();
    totalTimeLabel->setGeometry((screenWidth/2)+15, 20, 200, 50);
    totalTimeLabel->setStyleSheet("background: black; border: none; color: white;");
    totalTimeLabel->setFont(font);
    totalTimeLabel->setParent(this);
    totalTimeLabel->hide();

    bitRateLabel = new QLabel();
    bitRateLabel->setGeometry(screenWidth-430, 20, 200, 50);
    bitRateLabel->setStyleSheet("background: black; border: none; color: white;");
    bitRateLabel->setFont(font);
    bitRateLabel->setParent(this);
    bitRateLabel->hide();

    peersLabel = new QLabel();
    peersLabel->setGeometry(screenWidth-240, 20, 230, 50);
    peersLabel->setStyleSheet("background: black; border: none; color: white;");
    peersLabel->setFont(font);
    peersLabel->setParent(this);
    peersLabel->hide();

    bar = new QProgressBar();
    bar->setGeometry(0, 77, screenWidth, 3);
    bar->setStyleSheet (
            "QProgressBar { background: #000000; border: none; }"
            "QProgressBar::chunk { background: #9c9c9c; }"
         );
    bar->setTextVisible(false);
    bar->setMinimum(0);
    bar->setParent(this);
    bar->hide();

    sliderStartLoop = new QSlider(Qt::Horizontal);
    sliderStartLoop->setStyleSheet (
                        "QSlider::groove:horizontal { background: #141414; border: none; }"
                        "QSlider::sub-page:horizontal { background: white; }"
                        "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                     );
    sliderStartLoop->setMinimum(0);
    sliderStartLoop->setTracking(true);
    sliderStartLoop->setEnabled(false);
    sliderStartLoop->setParent(this);
    sliderStartLoop->hide();

    sliderStopLoop = new QSlider(Qt::Horizontal);
    sliderStopLoop->setStyleSheet (
                        "QSlider::groove:horizontal { background: #141414; border: none; }"
                        "QSlider::sub-page:horizontal { background: white; }"
                        "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                    );
    sliderStopLoop->setMinimum(0);
    sliderStopLoop->setTracking(true);
    sliderStopLoop->setEnabled(false);
    sliderStopLoop->setParent(this);
    sliderStopLoop->hide();
    
    slider = new QSlider(Qt::Horizontal);
    slider->setGeometry(0, screenHeight-15, screenWidth, 5);
    slider->setStyleSheet (
                "QSlider::groove:horizontal { background: #141414; border: none; }"
                "QSlider::sub-page:horizontal { background: white; }"
                "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
            );
    slider->setMinimum(0);
    slider->setTracking(true);
    slider->setEnabled(false);
    slider->setParent(this);
    slider->hide();

    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(set_currentTime(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(set_totalTime(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(set_slider_position(qint64)));
    connect(slider, SIGNAL(sliderPressed()), this, SLOT(slider_pressed()));
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(slider_moved(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(slider_released()));
    connect(sliderStartLoop, SIGNAL(sliderPressed()), this, SLOT(slider_startLoop_pressed()));
    connect(sliderStartLoop, SIGNAL(sliderMoved(int)), this, SLOT(slider_startLoop_moved(int)));
    connect(sliderStartLoop, SIGNAL(sliderMoved(int)), this, SLOT(set_sliderLoop_time(int)));
    connect(sliderStartLoop, SIGNAL(sliderReleased()), this, SLOT(slider_startLoop_released()));
    connect(sliderStopLoop, SIGNAL(sliderPressed()), this, SLOT(slider_stopLoop_pressed()));
    connect(sliderStopLoop, SIGNAL(sliderMoved(int)), this, SLOT(slider_stopLoop_moved(int)));
    connect(sliderStopLoop, SIGNAL(sliderMoved(int)), this, SLOT(set_sliderLoop_time(int)));
    connect(sliderStopLoop, SIGNAL(sliderReleased()), this, SLOT(slider_stopLoop_released()));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(status_change(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(error(QMediaPlayer::Error)));

    hideControlsDelay = new QTimer(this);
    hideControlsDelay->setSingleShot(true);
    connect(hideControlsDelay, SIGNAL(timeout()), this, SLOT(hide_controls()));

    setMouseTracking(true);
}

VideoPlayer::~VideoPlayer()
{
    _pig->disconnect(this);
    _torrent->disconnect(this);
    player->stop();
    delete player;
}

void VideoPlayer::sandbox(const QString *absoluteFilePath)
{
    onSandbox = true;
    buffering = false;
    skip = false;
    loop = false;
    paused = false;
    skip_key_value = 0;
    update_msec = 0;

    player->setMedia(QUrl::fromLocalFile(*absoluteFilePath));
    player->play();
}

void VideoPlayer::play_pause_stop()
{
    if (!loop) {
        if (player->state() == QMediaPlayer::PlayingState && !buffering) {
            player->pause();
            paused = true;
            pauseLabel->show();
        } else if (player->state() == QMediaPlayer::PausedState && !buffering) {
            player->play();
            paused = false;
            pauseLabel->hide();
        } else if (player->state() == QMediaPlayer::StoppedState) {
            if (update_msec == 0) {
                player->setPosition(qint64(0));
                player->play();
            } else {
                slider->setEnabled(true);
                update_msec = update_msec+5000;
                player->setPosition(update_msec);
                player->play();
            }
        } 
    }
}

void VideoPlayer::set_volume(int volume_key_value)
{
    if ((volume > 0 && volume < 100) || (volume == 0 && volume_key_value > 0) || (volume == 100 && volume_key_value < 0)) {
        volume = volume+volume_key_value;
        player->setVolume(volume);
        volumeLabel->setText(QString::number(volume));
        if (volume > 0) {
            volumePixmap.load(":/images/player/volume.png");
            volumeIcon->setPixmap(volumePixmap);
        } else {
            volumePixmap.load(":/images/player/volumeOff.png");
            volumeIcon->setPixmap(volumePixmap);
        }
    }
}

void VideoPlayer::set_loop()
{
    if (!buffering) {
        if (sliderStartLoop->isHidden()) {
            loop = true;
            paused = true;
            slider->setEnabled(false);
            slider->hide();
            pauseLabel->hide();
            player->pause();
            sliderStartLoop->setGeometry(0, screenHeight-23, screenWidth, 5);
            sliderStartLoop->setMaximum(player->duration());
            sliderStartLoop->setValue(0);
            sliderStopLoop->setGeometry(0, screenHeight-15, screenWidth, 5);
            sliderStopLoop->setMaximum(player->duration());
            sliderStopLoop->setValue(0);
            startLoop_msec = 0;
            stopLoop_msec = 0;
            set_sliderLoop_time(0);
            startLoopTimeLabel->show();
            stopLoopTimeLabel->show();
            sliderStartLoop->setEnabled(true);
            sliderStartLoop->show();
            sliderStopLoop->show();
        } else {
            loop = false;
            paused = false;
            sliderStartLoop->setEnabled(false);
            sliderStopLoop->setEnabled(false);
            slider->setEnabled(true);
            slider->show();
            startLoopTimeLabel->hide();
            stopLoopTimeLabel->hide();
            sliderStartLoop->hide();
            sliderStopLoop->hide();
            player->play();
        }
    }
}

void VideoPlayer::slider_startLoop_pressed()
{
    sliderStopLoop->setEnabled(false);
    hideControlsDelay->stop();
}

void VideoPlayer::slider_startLoop_moved(int position)
{
    sliderStartLoop->setValue(position);
    if(position < readyToRead_msec)
        player->setPosition(position);
    
    if (sliderStartLoop->value() > readyToRead_msec || (sliderStartLoop->value() > sliderStopLoop->value() && stopLoop_msec != 0)) {
        sliderStartLoop->setStyleSheet (
                            "QSlider::groove:horizontal { background: #141414; border: none; }"
                            "QSlider::sub-page:horizontal { background: red; }"
                            "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                         );
    } else {
        sliderStartLoop->setStyleSheet (
                            "QSlider::groove:horizontal { background: #141414; border: none; }"
                            "QSlider::sub-page:horizontal { background: white; }"
                            "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                         );
    }
}

void VideoPlayer::slider_startLoop_released()
{
    startLoop_msec = sliderStartLoop->value();

    if (startLoop_msec >= readyToRead_msec) {
         if (stopLoop_msec == 0) {
             startLoop_msec = readyToRead_msec-1500;
             sliderStartLoop->setValue(startLoop_msec);
         }
    } 
    if (startLoop_msec >= stopLoop_msec && stopLoop_msec != 0) {
         startLoop_msec = stopLoop_msec-1000;
         sliderStartLoop->setValue(startLoop_msec);
    }

    if (stopLoop_msec == 0) {
        sliderStartLoop->setEnabled(false);
        sliderStopLoop->setEnabled(true);
    } else {
        sliderStopLoop->setEnabled(true);
        player->setPosition(startLoop_msec);
        hideControlsDelay->start();
    }
    sliderStartLoop->setStyleSheet (
                        "QSlider::groove:horizontal { background: #141414; border: none; }"
                        "QSlider::sub-page:horizontal { background: white; }"
                        "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                     );
}

void VideoPlayer::slider_stopLoop_pressed()
{
    if(sliderStopLoop->isEnabled()) {
        hideControlsDelay->stop();
        sliderStartLoop->setEnabled(false);
    }
}

void VideoPlayer::slider_stopLoop_moved(int position)
{
    sliderStopLoop->setValue(position);
    if(position < readyToRead_msec)
        player->setPosition(position);

    if (sliderStopLoop->value() > readyToRead_msec || (sliderStopLoop->value() < sliderStartLoop->value())) {
        sliderStopLoop->setStyleSheet (
                            "QSlider::groove:horizontal { background: #141414; border: none; }"
                            "QSlider::sub-page:horizontal { background: red; }"
                            "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                        );
    } else {
        sliderStopLoop->setStyleSheet (
                            "QSlider::groove:horizontal { background: #141414; border: none; }"
                            "QSlider::sub-page:horizontal { background: white; }"
                            "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                        );
    }
}

void VideoPlayer::slider_stopLoop_released()
{
    stopLoop_msec = sliderStopLoop->value();

    if (stopLoop_msec > readyToRead_msec) {
        stopLoop_msec = readyToRead_msec-1500;
        sliderStopLoop->setValue(stopLoop_msec);
    }
    if (stopLoop_msec <= startLoop_msec) {
        stopLoop_msec = startLoop_msec+1000;
        sliderStopLoop->setValue(stopLoop_msec);
    }
    sliderStartLoop->setGeometry(0, 96, screenWidth, 5);
    sliderStopLoop->setGeometry(0, 104, screenWidth, 5);
    player->setPosition(startLoop_msec);
    player->play();
    slider->show();
    paused = false;
    sliderStartLoop->setEnabled(true);
    hideControlsDelay->start();
    sliderStopLoop->setStyleSheet (
                        "QSlider::groove:horizontal { background: #141414; border: none; }"
                        "QSlider::sub-page:horizontal { background: white; }"
                        "QSlider::handle:horizontal { background: black; border: 1px solid white; width: 13px; }"
                    );
}

void VideoPlayer::set_sliderLoop_time(int msecs)
{
    const int hours = msecs/(1000*60*60);
    const int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    const int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    QString formattedTime;
    formattedTime.append(QString( "%1" ).arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

    if (sliderStartLoop->value() == 0 && sliderStopLoop->value() == 0) {
        startLoopTimeLabel->setText(formattedTime);
        stopLoopTimeLabel->setText(formattedTime);
    } else if (sliderStartLoop->isEnabled()) {
        startLoopTimeLabel->setText(formattedTime);
    } else if (!sliderStartLoop->isEnabled()) {
        stopLoopTimeLabel->setText(formattedTime);
    }
}

void VideoPlayer::set_currentTime(qint64 msecs)
{
    const int hours = msecs/(1000*60*60);
    const int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    const int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    QString formattedTime;
    formattedTime.append(QString( "%1" ).arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

    currentTimeLabel->setText(formattedTime);
}

void VideoPlayer::set_totalTime(qint64 msecs)
{
    bar->setMaximum(msecs);
    slider->setRange(0, msecs);

    const int hours = msecs/(1000*60*60);
    const int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    const int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    QString formattedTime;
    formattedTime.append(QString( "%1" ).arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

    totalTimeLabel->setText(formattedTime);
}

void VideoPlayer::download_Information(int bitRate, int peers)
{
    bitRateLabel->setText(QString::number(bitRate)+"Kb/s");
    peersLabel->setText("PEERS "+QString::number(peers));
}

void VideoPlayer::progress(qint64 total_kb=0, qint64 downloaded_kb=0, int downloaded_offset=0)
{
    if (!skip && !onSandbox) {
        readyToRead_msec = ((downloaded_kb-13312)*player->duration())/total_kb;
        if ((player->duration()-(((downloaded_kb+13312)*player->duration())/total_kb)) <= 0) //TODO: Revisar.
            readyToRead_msec = player->duration();
        bar->setValue(readyToRead_msec);
        
        if (!paused) {
            if (slider->value() < readyToRead_msec) {
                if(player->state() == QMediaPlayer::PausedState) 
                    buffering = false;
            } else {
                if(player->state() == QMediaPlayer::PlayingState) {
                    buffering = true;
                    onBuffering();
                }
            }
        }

        if (loop && player->state() == 1)
            if (slider->value() >= stopLoop_msec)
                player->setPosition(startLoop_msec);
        //
        qDebug() << "SLIDER_MSEC: " << slider->value();
        qDebug() << "READY__MSEC: " << readyToRead_msec;
        //
    } else if (!onSandbox) {
        barOffset->setValue(downloaded_offset);
    }
}

void VideoPlayer::onBuffering()
{
    static int i = 0;
    if (i%2 == 0)
        witnessBufferLabel->setStyleSheet("background: black; border: none; color: white;");
    else
        witnessBufferLabel->setStyleSheet("background: black; border: none; color: black;");

    if (buffering) {
        if (i==0) {
            player->pause();
            show_controls();
            bufferLabel->show();
            witnessBufferLabel->show();
        }
        ++i;
        QTimer::singleShot(500, this, SLOT(onBuffering()));
    } else {
        i = 0;
        bufferLabel->hide();
        witnessBufferLabel->hide();
        hideControlsDelay->start();
        player->play();
    }
}


void VideoPlayer::update_player()
{
    barOffset->hide();
    slider->setEnabled(true);
    hideControlsDelay->start();

    update_msec = slider->value(); 
    player->setPosition(update_msec);
    player->play();
    slider->setStyleSheet (
                "QSlider::groove:horizontal { background: #141414; border: none; }"
                "QSlider::sub-page:horizontal { background: white; }"
                "QSlider::handle:horizontal { background: black; border: 1px solid #ffffff; width: 13px; }"
            );
    skip = false;
    bar->show();

    qDebug() << "-- UPDATE_PLAYER_MSEC: " << update_msec;
}

void VideoPlayer::set_slider_position(qint64 position)
{
    slider->setValue(position);
}

void VideoPlayer::slider_pressed()
{
    skip = true;
    hideControlsDelay->stop();
    player->pause();
}

void VideoPlayer::slider_moved(int position)
{
    slider->setValue(position);
    set_currentTime(qint64(slider->value()));
    if (slider->value() > bar->value()) {
        slider->setStyleSheet (
                    "QSlider::groove:horizontal { background: #141414; border: none; }"
                    "QSlider::sub-page:horizontal { background: red; }"
                    "QSlider::handle:horizontal { background: black; border: 1px solid #ffffff; width: 13px; }"
                );
    } else {
        slider->setStyleSheet (
                    "QSlider::groove:horizontal { background: #141414; border: none; }"
                    "QSlider::sub-page:horizontal { background: white; }"
                    "QSlider::handle:horizontal { background: black; border: 1px solid #ffffff; width: 13px; }"
                );
    }
}

void VideoPlayer::slider_released()
{
    const qint64 total_msec = player->duration();
    const qint64 offset_msec = slider->value()+skip_key_value;
    bool available = false;                               

    if (!loop) {
        QMetaObject::invokeMethod(_torrent, "piece_is_available", Qt::DirectConnection, Q_RETURN_ARG(bool, available), Q_ARG(qint64, total_msec), Q_ARG(qint64, offset_msec));
        if ((slider->value() < bar->value()) && (offset_msec < readyToRead_msec) && available) {
            if (skip_key_value != 0) {
                player->setPosition(offset_msec);
            } else {
                skip = false;
                player->setPosition(qint64(slider->value()));
                player->play();
                hideControlsDelay->start();
            }
        } else if (skip_key_value == 0 && !available) {
            bar->hide();
            pauseLabel->hide();
            barOffset->show();
            slider->setEnabled(false);
            slider->setStyleSheet (
                        "QSlider::groove:horizontal { background: #141414; border: none; }"
                        "QSlider::sub-page:horizontal { background: #9c9c9c; }"
                        "QSlider::handle:horizontal { background: black; border: 1px solid #ffffff; width: 13px; }"
                    );
            QMetaObject::invokeMethod(_torrent, "piece_update", Qt::DirectConnection, Q_ARG(qint64, total_msec), Q_ARG(qint64, offset_msec));
        } 
    }
    skip_key_value = 0;
    buffering = false;
}

void VideoPlayer::show_controls()
{
    hideControlsDelay->stop();
    box->show();
    volumeIcon->show();
    volumeLabel->show();
    currentTimeLabel->show();
    totalTimeLabel->show();
    bitRateLabel->show();
    peersLabel->show();
    bar->show();
    slider->show();
    if (buffering) {
        bufferLabel->show();
        witnessBufferLabel->show();
    }
    if (loop) {
        startLoopTimeLabel->show();
        stopLoopTimeLabel->show();
        sliderStartLoop->show();
        sliderStopLoop->show();
    }
}

void VideoPlayer::hide_controls()
{
    if ((!loop && !paused) || (loop && player->state() == 1)) {
        box->hide();
        volumeIcon->hide();
        volumeLabel->hide();
        pauseLabel->hide();
        bufferLabel->hide();
        witnessBufferLabel->hide();
        startLoopTimeLabel->hide();
        stopLoopTimeLabel->hide();
        currentTimeLabel->hide();
        totalTimeLabel->hide();
        bitRateLabel->hide();
        peersLabel->hide();
        sliderStartLoop->hide();
        sliderStopLoop->hide();
        bar->hide();
        slider->hide();
    }
}

void VideoPlayer::keyPressEvent(QKeyEvent *event)
{
    if (!skip) {
        show_controls();
        if (!buffering)
            hideControlsDelay->start();

        if (event->key() == Qt::Key_Space) {
            play_pause_stop();
        } else if (event->key() == Qt::Key_Left) {
            skip_key_value = -10000;
            slider_released();
        } else if (event->key() == Qt::Key_Right) {
            skip_key_value = 10000;
            slider_released();
        } else if (event->key() == Qt::Key_Up) {
            set_volume(5);
        } else if (event->key() == Qt::Key_Down) {
            set_volume(-5);
        } else if (event->key() == Qt::Key_L) {
            set_loop();
        }
    }
    if (!onSandbox) {
        if ((event->key() == (Qt::Key_Escape)) && !(event->modifiers() & Qt::ControlModifier)) {
            QMetaObject::invokeMethod(_pig, "player_handle", Qt::QueuedConnection, Q_ARG(QString, ""), Q_ARG(bool, false), Q_ARG(bool, false), Q_ARG(bool, false), Q_ARG(bool, true));
        } else if ((event->key() == (Qt::Key_Escape)) && (event->modifiers() & Qt::ControlModifier)) {
            this->~VideoPlayer();
            QMetaObject::invokeMethod(_pig, "quit", Qt::QueuedConnection);
        }
    }
    event->accept();
}

void VideoPlayer::mousePressEvent(QMouseEvent *event)
{
    show_controls();
    if (!buffering)
        hideControlsDelay->start();
    event->accept();
}

void VideoPlayer::status_change(QMediaPlayer::MediaStatus status)
{
    qDebug() << "-- STATUS: " << status;
    //if (status == QMediaPlayer::LoadingMedia) {
        //if (onSandbox)
            // TODO: Que se fije si quedo colgado en este estado y llame a play_pause_stop
    //}
    if (status == QMediaPlayer::BufferedMedia) { // TODO: Puede que este en Buffered y despues de error.
        if (onSandbox) {
            onSandbox = false;
            slider->setEnabled(true);
            QMetaObject::invokeMethod(_pig, "player_handle", Qt::QueuedConnection, Q_ARG(QString, ""), Q_ARG(bool, false), Q_ARG(bool, false), Q_ARG(bool, true), Q_ARG(bool, false));
            this->setFocus();
            box->show();
            volumeIcon->show();
            volumeLabel->show();
            currentTimeLabel->show();
            totalTimeLabel->show();
            bitRateLabel->show();
            peersLabel->show();
            bar->show();
            slider->show();
            hideControlsDelay->start(3000);
        } else if (skip) {
            skip = false;
            hideControlsDelay->start();
        }
    } else if (status == QMediaPlayer::UnknownMediaStatus || status == QMediaPlayer::NoMedia || status == QMediaPlayer::InvalidMedia || status == QMediaPlayer::EndOfMedia) {
        if(update_msec == 0) {
            QTimer::singleShot(5000, this, SLOT(play_pause_stop()));
        } else {
            skip = true;
            slider->setEnabled(false);
            show_controls();
            QTimer::singleShot(1000, this, SLOT(play_pause_stop()));
        }
    }
}

void VideoPlayer::error(QMediaPlayer::Error)
{
    qDebug() << "-- ERROR: " << player->QMediaPlayer::error();
}
