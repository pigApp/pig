#ifndef QT_NO_OPENGL
#include <QtOpenGL/QGLWidget>
#endif
#include <QtAV/OSDFilter.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "videoplayer.h"

using namespace QtAV;

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
{
    renderer.widget()->setFixedWidth(1920);
    renderer.widget()->setFixedHeight(1080);
    renderer.osdFilter()->setShowType(QtAV::OSD::ShowNone);
    mediaPlayer.setRenderer(&renderer);

    play_pause_Button = new QPushButton();
    play_pause_Button->setGeometry(2, 1048, 30, 30);
    play_pause_Button->setStyleSheet("background: transparent; border: none");
    play_pause_Button->setIconSize(QSize(30, 30));
    play_pause_Button->setParent(renderer.widget());
    play_pause_Button->setEnabled(false);
    play_pause_Button->hide();

    stopButton = new QPushButton();
    stopButton->setGeometry(34, 1048, 30, 30);
    stopButton->setStyleSheet("background: transparent; border: none");
    stopButton->setIcon(QIcon("://images/player/stop.png"));
    stopButton->setIconSize(QSize(30, 30));
    stopButton->setParent(renderer.widget());
    stopButton->setEnabled(false);
    stopButton->hide();

    slider = new QSlider(Qt::Horizontal);
    slider->setGeometry(118, 1063, 1750, 5); // TODO: Ancho
    slider->setStyleSheet("background: white; border: none");
    slider->setTracking(true);
    slider->setMinimum(0);
    slider->setParent(renderer.widget());
    slider->setEnabled(false);
    slider->hide();

    volumeButton = new QPushButton();
    volumeButton->setGeometry(76, 1048, 30, 30);
    volumeButton->setStyleSheet("background: transparent; border: none");
    volumeButton->setIcon(QIcon("://images/player/headphone.png"));
    volumeButton->setIconSize(QSize(30, 30));
    volumeButton->setParent(renderer.widget());
    volumeButton->setEnabled(false);
    volumeButton->hide();

    volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setGeometry(88, 947, 5, 100);
    volumeSlider->setStyleSheet("background: white; border: none");
    volumeSlider->setTracking(true);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setParent(renderer.widget());
    volumeSlider->setEnabled(false);
    volumeSlider->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(renderer.widget());
    setLayout(layout);

    connect(play_pause_Button, SIGNAL(clicked()), this, SLOT(play_pause()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(onPositionSliderChange()));
    connect(&mediaPlayer, SIGNAL(started()), this, SLOT(onStartPlay()));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChange(qint64)));
    connect(volumeButton, SIGNAL(clicked()), this, SLOT(enable_disable_audio()));
    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume()));

    // mediaPlayer.play("/home/ok200/pigVideos/trailer_1080p.ogg.mov");
    // happyFeet "http://easyhtml5video.com/images/happyfit2.mp4"
    // animacion "http://p.demo.flowplayer.netdna-cdn.com/vod/demo.flowplayer/bbb-800.mp4"
    // local "/home/ok200/pigVideos/trailer_1080p.ogg.mov"
}

VideoPlayer::~VideoPlayer()
{
    if(mediaPlayer.isPlaying() || mediaPlayer.isPaused())
        mediaPlayer.stop();
}

void VideoPlayer::open(const QString &file)
{
    mediaPlayer.play(file);
}

void VideoPlayer::play_pause()
{
    if (mediaPlayer.isPaused()) {
        mediaPlayer.pause(false);
        mediaPlayer.seek(qint64(currentPosition));
        play_pause_Button->setIcon(QIcon("://images/player/pause.png"));
    } else {
        mediaPlayer.pause(true);
        currentPosition = mediaPlayer.position();
        play_pause_Button->setIcon(QIcon("://images/player/play.png"));
    }
}

void VideoPlayer::stop()
{
    mediaPlayer.stop();
}

void VideoPlayer::onStartPlay()
{
    play_pause_Button->setIcon(QIcon("://images/player/pause.png"));
    play_pause_Button->setEnabled(true);
    play_pause_Button->show();
    stopButton->setEnabled(true);
    stopButton->show();
    slider->setMaximum(mediaPlayer.duration());
    slider->setValue(0);
    slider->setEnabled(true);
    slider->show();
    volumeButton->setEnabled(true);
    volumeButton->show();
    volumeSlider->setValue(0); // TODO: Setear a 50.
    volumeSlider->setEnabled(true);
    volumeSlider->show();
    setVolume();
}

void VideoPlayer::onPositionChange(qint64 position)
{
    slider->setValue(position);
}

void VideoPlayer::onPositionSliderChange()
{
    mediaPlayer.seek(qint64(slider->value())); // seek o setPosition.
}

void VideoPlayer::enable_disable_audio()
{
    if (mediaPlayer.isMute())
        mediaPlayer.setMute(false);
    else
        mediaPlayer.setMute(true);
}

void VideoPlayer::setVolume()
{
    AudioOutput *ao = mediaPlayer.audio();
    qreal v = qreal(volumeSlider->value()); // TODO: Falta calculo para que el volumen no sature.
    if (ao)
        ao->setVolume(v);
}

void VideoPlayer::setAudioOutput(AudioOutput* ao)
{
    Q_UNUSED(ao);
}
