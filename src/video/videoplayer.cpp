#include <QGraphicsView>
#ifndef QT_NO_OPENGL
#include <QtOpenGL/QGLWidget>
#endif
#include <QVBoxLayout>
#include <QDebug>

#include "videoplayer.h"

using namespace QtAV;

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent), videoItem(0)
{
    videoItem = new GraphicsItemRenderer;
    videoItem->resizeRenderer(1918, 1078);
    videoItem->setOutAspectRatioMode(VideoRenderer::RendererAspectRatio);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(videoItem);

    QGraphicsView *graphicsView = new QGraphicsView(scene);
    #if 0
    #ifndef QT_NO_OPENGL
        QGLWidget *glw = new QGLWidget(QGLFormat(QGL::SampleBuffers));
        glw->setAutoFillBackground(false);
        graphicsView->setCacheMode(QGraphicsView::CacheNone);
        graphicsView->setViewport(glw);
    #endif
    #endif //0

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(graphicsView);
    setLayout(layout);

    mediaPlayer.setRenderer(videoItem);

    play_pauseButton = new QPushButton();
    play_pauseButton->setGeometry(2, 1048, 30, 30);
    play_pauseButton->setParent(graphicsView);
    play_pauseButton->setIconSize(QSize(30, 30));
    play_pauseButton->setStyleSheet("background: transparent; border: none");
    play_pauseButton->hide();

    stopButton = new QPushButton();
    stopButton->setGeometry(34, 1048, 30, 30);
    stopButton->setParent(graphicsView);
    stopButton->setIcon(QIcon("://images/player/stop.png"));
    stopButton->setIconSize(QSize(30, 30));
    stopButton->setStyleSheet("background: transparent; border: none");
    stopButton->hide();

    loopButton = new QPushButton();
    loopButton->setGeometry(66, 1048, 30, 30);
    loopButton->setParent(graphicsView);
    loopButton->setIcon(QIcon("://images/player/loop.png"));
    loopButton->setIconSize(QSize(30, 30));
    loopButton->setStyleSheet("background: transparent; border: none");
    loopButton->hide();

    volumeButton = new QPushButton();
    volumeButton->setGeometry(106, 1048, 30, 30);
    volumeButton->setParent(graphicsView);
    volumeButton->setIcon(QIcon("://images/player/headphone.png"));
    volumeButton->setIconSize(QSize(30, 30));
    volumeButton->setStyleSheet("background: transparent; border: none");
    volumeButton->hide();

    connect(play_pauseButton, SIGNAL(clicked()), this, SLOT(play_pause()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
}

VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::open(const QString &file)
{
    mediaPlayer.play(file);

    play_pauseButton->setIcon(QIcon("://images/player/pause.png")); // Esto debe ir cuando inicia el video void started().
    play_pauseButton->show();
    loopButton->show();
    stopButton->show();
    volumeButton->show();
}

void VideoPlayer::play_pause()
{
    if (mediaPlayer.isPaused()) {
        mediaPlayer.pause(false);
        play_pauseButton->setIcon(QIcon("://images/player/pause.png"));
        qDebug() << "paused: " << mediaPlayer.isPaused();
    } else {
        mediaPlayer.pause(true);
        play_pauseButton->setIcon(QIcon("://images/player/play.png"));
        qDebug() << "paused: " << mediaPlayer.isPaused();
    }
}

void VideoPlayer::stop()
{
    mediaPlayer.stop();
}

