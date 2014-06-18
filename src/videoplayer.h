#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent, QObject *obj, int screenWidth, int screenHeight);
    ~VideoPlayer();

    QMediaPlayer *player;

public slots:
    bool availableFile();
    void update();

private slots:
    void playPause();
    void setCurrentTime(qint64 msecs);
    void setTotalTime(qint64 msecs);
    void setSliderPosition(qint64 position);
    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();
    void setPositiveVolume();
    void setNegativeVolume();
    void statusChange();
    void error(QMediaPlayer::Error);
    void buffered();

private:
    QObject *_torrent;

    QVideoWidget *videoWidget;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QSlider *slider;

    bool init;
    int volume;
};
#endif
