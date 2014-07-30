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

    barOffset = new QProgressBar();
    barOffset->setGeometry(170, 45, screenWidth/13, 3);
    barOffset->setStyleSheet (
        "QProgressBar { background: #141414; border: none; }"
        "QProgressBar::chunk { background: white; }"
    );
    barOffset->setTextVisible(false);
    barOffset->setMinimum(0);
    barOffset->setMaximum(20);
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
    bar->setGeometry(3.4, screenHeight-7, screenWidth-8.3, 3);
    bar->setStyleSheet (
                "QProgressBar { background: #141414; border: none; }"
                "QProgressBar::chunk { background: #9c9c9c; }"
    );
    bar->setTextVisible(false);
    bar->setMinimum(0);
    bar->setParent(this);
    bar->hide();
    
    slider = new QSlider(Qt::Horizontal);
    slider->setGeometry(4, screenHeight-15, screenWidth-9, 5);
    slider->setStyleSheet (
        "QSlider::groove:horizontal { background: #141414; border: none; }"
        "QSlider::sub-page:horizontal { background: white; }"
        "QSlider::handle:horizontal { background: black; border: 1px solid #ffffff; width: 13px; }"
    );
    slider->setMinimum(0);
    slider->setTracking(true);
    slider->setEnabled(false);
    slider->setParent(this);
    slider->hide();

    loopIcon = new QLabel();
    loopPixmap.load((":/images/player/loop.png"));
    loopIcon->setPixmap(loopPixmap);
    loopIcon->setGeometry(170, 30, 30, 30);
    loopIcon->setStyleSheet("background: black; border: none;");
    loopIcon->setParent(this);
    loopIcon->hide();

    loopLabel = new QLabel();
    loopLabel->setStyleSheet("background: black; border: none; color: white;");
    loopLabel->setFont(font);
    loopLabel->setParent(this);
    loopLabel->hide();
    
    loopTimeLabel = new QLabel();
    loopTimeLabel->setStyleSheet("background: black; border: none; color: white;");
    loopTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    loopTimeLabel->setMargin(1);
    loopTimeLabel->setFont(font);
    loopTimeLabel->setParent(this);
    loopTimeLabel->hide();

    sliderStartLoop = new QSlider(Qt::Horizontal);
    sliderStartLoop->setGeometry(4, screenHeight-38, screenWidth-9, 5);
    sliderStartLoop->setStyleSheet (
        "QSlider::groove:horizontal { background: #141414; border: none; }"
        "QSlider::sub-page:horizontal { background: yellow; }"
        "QSlider::handle:horizontal { background: yellow; border: 1px solid yellow; width: 13px; }"
    );
    sliderStartLoop->setMinimum(0);
    sliderStartLoop->setTracking(true);
    sliderStartLoop->setEnabled(false);
    sliderStartLoop->setParent(this);
    sliderStartLoop->hide();

    sliderStopLoop = new QSlider(Qt::Horizontal);
    sliderStopLoop->setGeometry(4, screenHeight-32, screenWidth-9, 5);
    sliderStopLoop->setStyleSheet (
        "QSlider::groove:horizontal { background: #141414; border: none; }"
        "QSlider::sub-page:horizontal { background: yellow; }"
        "QSlider::handle:horizontal { background: yellow; border: 1px solid yellow; width: 13px; }"
    );
    sliderStopLoop->setMinimum(0);
    sliderStopLoop->setTracking(true);
    sliderStopLoop->setEnabled(false);
    sliderStopLoop->setParent(this);
    sliderStopLoop->hide();

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

    hideControlsTimer = new QTimer(this);
    hideControlsTimer->setSingleShot(true);
    connect(hideControlsTimer, SIGNAL(timeout()), this, SLOT(hide_controls()));

    setMouseTracking(true);
}

VideoPlayer::~VideoPlayer()
{
    _pig->disconnect(this);
    _torrent->disconnect(this);
    player->stop();
    delete player;
}

void VideoPlayer::sandbox(QString absoluteFilePath)
{
    onSandbox = true;
    skip = false;
    loop = false;
    paused = false;
    skip_key_value = 0;

    player->setMedia(QUrl::fromLocalFile(absoluteFilePath));
    player->play();
}

void VideoPlayer::play_pause_stop()
{
    if (!loop) {
        if (player->state() == QMediaPlayer::PlayingState) {
            player->pause();
            paused = true;
            pauseLabel->show();
            hideControlsTimer->stop();
        } else if (player->state() == QMediaPlayer::PausedState && !skip) {
            player->play();
            paused = false;
            pauseLabel->hide();
        } else if (player->state() == QMediaPlayer::PausedState && skip) {
            skip = false;
            barOffset->hide();
            player->setPosition(skip_player_msec);
            player->play();
            hideControlsTimer->start();
        } else if (player->state() == QMediaPlayer::StoppedState) {
            player->setPosition(qint64(0));
            player->play();
        }
    }
}

void VideoPlayer::standBy()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else if (player->state() == QMediaPlayer::PausedState) {
        player->setPosition(skip_player_msec);
        player->play();
        hideControlsTimer->start();
    } else if (player->state() == QMediaPlayer::StoppedState) {
        player->setPosition(qint64(slider->value()-1000));
        player->play();
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
    if (sliderStartLoop->isHidden()) {
        player->pause();
        loop = true;
        paused = true;
        slider->setDisabled(true);
        startLoop_msec = 0;
        stopLoop_msec = 0;
        pauseLabel->hide();
        loopIcon->show();
        loopLabel->setGeometry(4, screenHeight-100, 255, 50);
        loopLabel->setText("START LOOP");
        loopLabel->show();
        loopTimeLabel->setGeometry(255, screenHeight-100, 197, 50);
        set_sliderLoop_time(0);
        sliderStartLoop->setMaximum(player->duration());
        sliderStartLoop->setValue(startLoop_msec);
        sliderStartLoop->setEnabled(true);
        sliderStartLoop->show();
        sliderStopLoop->setMaximum(player->duration());
        sliderStopLoop->setValue(stopLoop_msec);
        sliderStopLoop->show();
    } else {
        loop = false;
        paused = false;
        loopIcon->hide();
        loopLabel->hide();
        loopTimeLabel->hide();
        sliderStartLoop->setEnabled(false);
        sliderStartLoop->hide();
        sliderStopLoop->setEnabled(false);
        sliderStopLoop->hide();
        player->play();
        slider->setDisabled(false);
    }
}

void VideoPlayer::slider_startLoop_pressed()
{
    hideControlsTimer->stop();
}

void VideoPlayer::slider_startLoop_moved(int position)
{
    sliderStartLoop->setValue(position);
}

void VideoPlayer::slider_startLoop_released()
{
    startLoop_msec = sliderStartLoop->value();

    if (startLoop_msec >= readyToRead_msec && !sliderStopLoop->isEnabled()) {
         startLoop_msec = readyToRead_msec-120000;
         sliderStartLoop->setValue(startLoop_msec);
    } else if (startLoop_msec >= stopLoop_msec && sliderStopLoop->isEnabled()) {
         startLoop_msec = stopLoop_msec-10000;
         sliderStartLoop->setValue(startLoop_msec);
    }

    if (loopLabel->text() == "START LOOP") {
        loopLabel->setGeometry(4, screenHeight-100, 210, 50);
        loopLabel->setText("END LOOP");
        loopTimeLabel->setGeometry(210, screenHeight-100, 197, 50);
        set_sliderLoop_time(0);
        sliderStartLoop->setEnabled(false);
        sliderStopLoop->setEnabled(true);
    } else {
        player->setPosition(startLoop_msec);
        hideControlsTimer->start();
        loopTimeLabel->hide();
    }
}

void VideoPlayer::slider_stopLoop_pressed()
{
    hideControlsTimer->stop();
}

void VideoPlayer::slider_stopLoop_moved(int position)
{
    sliderStopLoop->setValue(position);
}

void VideoPlayer::slider_stopLoop_released()
{
    stopLoop_msec = sliderStopLoop->value();

    if (stopLoop_msec > readyToRead_msec) {
        stopLoop_msec = readyToRead_msec-120000;
        sliderStopLoop->setValue(stopLoop_msec);
    }
    if (stopLoop_msec <= startLoop_msec) {
        stopLoop_msec = startLoop_msec+10000;
        sliderStopLoop->setValue(stopLoop_msec);
    }
    player->setPosition(startLoop_msec);
    player->play();
    paused = false;
    loopTimeLabel->hide();
    hideControlsTimer->start();
    if (loopLabel->text() != "") {
        loopLabel->setText("");
        loopLabel->hide();
        sliderStartLoop->setEnabled(true);
    }
}

void VideoPlayer::set_sliderLoop_time(int msecs)
{
    int hours = msecs/(1000*60*60);
    int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    QString formattedTime;
    formattedTime.append(QString( "%1" ).arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

    loopTimeLabel->setText(formattedTime);
    if (loopTimeLabel->isHidden()) {
        if (loopLabel->text() == "")
            loopTimeLabel->setGeometry(4, screenHeight-100, 177, 50);
        loopTimeLabel->show();
    }
}

void VideoPlayer::set_currentTime(qint64 msecs)
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

void VideoPlayer::set_totalTime(qint64 msecs)
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

void VideoPlayer::download_Information(int bitRate, int peers)
{
    bitRateLabel->setText(QString::number(bitRate)+"Kb/s");
    peersLabel->setText("PEERS "+QString::number(peers));
}

void VideoPlayer::progress(int total_kb=0, int downloaded_kb=0, int downloaded_offset=0)
{
    if (!skip) {
        readyToRead_msec = ((downloaded_kb-12288)*player->duration())/total_kb; //8192
        bar->setValue(readyToRead_msec); // TODO: Si el torrent termino la descarga(señal enviada desde Torrent), forzar la barra a estar cargada totalmente.
        
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

        if (loop && player->state() == 1)
            if (slider->value() >= stopLoop_msec)
                player->setPosition(startLoop_msec);

        qDebug() << "SLIDER_MSEC: " << slider->value();
        qDebug() << "READY__MSEC: " << readyToRead_msec;
    } else {
        barOffset->setValue(downloaded_offset);
    }
}

void VideoPlayer::update()
{
    //slider->setEnabled(true);
    qDebug() << "----UPDATE";
    skip_player_msec = slider->value()+1000;
    QTimer::singleShot(5000, this, SLOT(play_pause_stop()));
}

void VideoPlayer::set_slider_position(qint64 position)
{
    slider->setValue(position);
}

void VideoPlayer::slider_pressed()
{
    skip = true;
    hideControlsTimer->stop();
    player->pause();
}

void VideoPlayer::slider_moved(int position)
{
    slider->setValue(position);
    set_currentTime(qint64(slider->value()));
}

void VideoPlayer::slider_released()
{
    int total_msec = player->duration();
    int offset_msec = slider->value()+skip_key_value;
    bool available = false;

    if (!loop) {
        QMetaObject::invokeMethod(_torrent, "piece_is_available", Qt::DirectConnection, Q_RETURN_ARG(bool, available), Q_ARG(int, total_msec), Q_ARG(int, offset_msec));
        if (((slider->value()) < (bar->value())) && available) {
            if (skip_key_value != 0) {
                player->setPosition(qint64(slider->value()+skip_key_value));
                skip_key_value = 0;
            } else {
                skip = false;
                player->setPosition(qint64(slider->value()));
                player->play();
                hideControlsTimer->start();
            }
        } else if (skip_key_value == 0 && !available) {
            pauseLabel->hide();
            bar->setValue(qint64(slider->value()+4500));
            barOffset->show();
            //slider->setEnabled(false);
            QMetaObject::invokeMethod(_torrent, "piece_update", Qt::DirectConnection, Q_ARG(int, total_msec), Q_ARG(int, offset_msec));
        } else if (skip_key_value != 0 && !available) {
            skip_key_value = 0;
        }
    }
}

void VideoPlayer::show_controls()
{
    hideControlsTimer->stop();
    box->show();
    volumeIcon->show();
    volumeLabel->show();
    currentTimeLabel->show();
    totalTimeLabel->show();
    bitRateLabel->show();
    peersLabel->show();
    bar->show();
    slider->show();
    if (loop) {
        loopIcon->show();
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
        loopIcon->hide();
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
    if (!skip)  {
        hideControlsTimer->stop();
        show_controls();
        hideControlsTimer->start();

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
        } else if ((event->key() == (Qt::Key_Escape)) && !(event->modifiers() & Qt::ControlModifier)) {
            QMetaObject::invokeMethod(_pig, "player_handle", Qt::QueuedConnection, Q_ARG(QString, ""), Q_ARG(bool, false), Q_ARG(bool, false), Q_ARG(bool, false), Q_ARG(bool, true));
        } else if ((event->key() == (Qt::Key_Escape)) && (event->modifiers() & Qt::ControlModifier)) {
            this->~VideoPlayer();
            QMetaObject::invokeMethod(_pig, "quit", Qt::QueuedConnection);
        }
        event->accept();
    }
}

void VideoPlayer::mousePressEvent(QMouseEvent *event)
{
    hideControlsTimer->stop();
    show_controls();
    hideControlsTimer->start();
    event->accept();
}

void VideoPlayer::status_change(QMediaPlayer::MediaStatus status)
{
    qDebug() << "STATUS: " << status;
    //if (status == QMediaPlayer::LoadingMedia) {
        //if (onSandbox)
            // TODO: Que se fije si quedo colgado en este estado y llame a play_pause_stop
    //}
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
            hideControlsTimer->start(3000);
        } else if (!skip){
            player->play();
        }
        if (onSandbox) {
            onSandbox = false;
            QMetaObject::invokeMethod(_pig, "player_handle", Qt::QueuedConnection, Q_ARG(QString, ""), Q_ARG(bool, false), Q_ARG(bool, false), Q_ARG(bool, true), Q_ARG(bool, false));
            this->setFocus();
        }
    } else if (status == QMediaPlayer::UnknownMediaStatus || status == QMediaPlayer::NoMedia || status == QMediaPlayer::InvalidMedia || status == QMediaPlayer::EndOfMedia) {
        skip = false;
        QTimer::singleShot(5000, this, SLOT(play_pause_stop()));
    }
}

void VideoPlayer::error(QMediaPlayer::Error)
{
    qDebug() << "--ERROR: " << player->QMediaPlayer::error();
}
