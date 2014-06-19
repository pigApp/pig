#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class VideoPlayer : public QObject
{
    Q_OBJECT

public:
    explicit VideoPlayer(QObject *parent, int screenWidth, int screenHeight);
    ~VideoPlayer();

    Q_INVOKABLE void progress(int totalPieces, int currentPiece);
    Q_INVOKABLE void update();

    QObject *_torrent;
    QMediaPlayer *player;
    QVBoxLayout *layout;

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
    void statusChange(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error);

private:
    QVideoWidget *videoWidget;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QSlider *slider;

    bool init;
    int volume;
};
#endif
