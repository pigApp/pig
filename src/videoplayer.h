#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QLabel>
#include <QProgressBar>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>

class VideoPlayer : public QObject
{
    Q_OBJECT

public:
    explicit VideoPlayer(QObject *parent, int screenWidth, int screenHeight);
    ~VideoPlayer();

    Q_INVOKABLE void progress(int totalPieces, int availablePiece);
    Q_INVOKABLE void downloadInfo(int bitRate, int peers, int seeds);
    Q_INVOKABLE void update();

    QObject *_torrent;
    QVBoxLayout *layout;

public slots:
    void doRun(QString absoluteFilePath);

private:
    QVideoWidget *videoWidget;
    QMediaPlayer *player;
    QWidget *box;
    QLabel *totalTimeLabel;
    QLabel *currentTimeLabel;
    QLabel *bitRateLabel;
    QLabel *peersLabel;
    QLabel *seedsLabel;
    QProgressBar *bar;
    QSlider *slider;

    bool init;
    bool control;
    int volume;

    bool pausedForUser;

private slots:
    void playPause();
    void error(QMediaPlayer::Error);
    void statusChange(QMediaPlayer::MediaStatus status);
    void setCurrentTime(qint64 msecs);
    void setTotalTime(qint64 msecs);
    void setSliderPosition(qint64 position);
    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();
    void setPositiveVolume();
    void setNegativeVolume();

    void playPauseForUser();
};
#endif
