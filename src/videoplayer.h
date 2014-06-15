#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QWidget>

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent, int screenWidth, int screenHeight);
    ~VideoPlayer();

public slots:
    void open(const QString& file, QObject *obj);

signals:
    void offsetx(int offset);

private slots:
    void playPause();
    void setCurrentTime(qint64 time);
    void setTotalTime(qint64 time);
    void setSliderPosition(qint64 position);
    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();
    void setPositiveVolume();
    void setNegativeVolume();
    void statusChange();
    void test();

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
