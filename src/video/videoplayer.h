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
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();


public slots:
    void open(const QString& file);

private slots:
    void playPause();
    void stop();
    void statusChange();
    void positionChange(qint64 position);
    void positionSliderChange();
    void showHideVolumeSlider();
    void setVolume(int volume);

private:
    QVideoWidget *videoWidget;
    QMediaPlayer *player;

    QPushButton *playPauseButton;
    QPushButton *stopButton;
    QSlider *slider;
    QPushButton *volumeButton;
    QSlider *volumeSlider;
    QLabel *timeLabel;

    qint64 currentPosition;
};
#endif
