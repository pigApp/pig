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
    VideoPlayer(QWidget *parent, int screenWidth, int screenHeight);
    ~VideoPlayer();

public slots:
    void open(const QString& file, QObject *obj);
    void update();

signals:
    void offsetx(int offset);

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

private:
    QVideoWidget *videoWidget;
    QMediaPlayer *player;
    QObject *_torrent;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QSlider *slider;

    bool init;
    int volume;
};
#endif
