#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <src/video/QtAV/AVPlayer.h>
#include <src/video/QtAV/GraphicsItemRenderer.h>
#include <QPushButton>
#include <QWidget>

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    void open(const QString& file);

private slots:
    void play_pause();
    void stop();

private:
    QtAV::AVPlayer mediaPlayer;
    QtAV::GraphicsItemRenderer *videoItem;
    QPushButton *play_pauseButton;
    QPushButton *loopButton;
    QPushButton *stopButton;
    QPushButton *volumeButton;
};
#endif
