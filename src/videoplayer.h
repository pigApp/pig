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

    Q_INVOKABLE void downloadInfo(int bitRate, int peers, int seeds);
    Q_INVOKABLE void progress(int totalPieces, int currentPiece);
    Q_INVOKABLE void update();

    QObject *_torrent;
    QVBoxLayout *layout;

public slots:
    void run(QString absoluteFilePath, int totalPieces, int currentPiece);

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


private:
    QWidget *blackBox;
    QVideoWidget *videoWidget;
    QMediaPlayer *player;
    QLabel *bitRateLabel;
    QLabel *peersLabel;
    QLabel *seedsLabel;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QProgressBar *bar;
    QSlider *slider;

    bool init;
    bool check;
    int volume;

    int test;//
};
#endif
