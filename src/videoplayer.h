#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QPixmap>
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

    Q_INVOKABLE void downloadInfo(int bitRate, int peers);
    Q_INVOKABLE void progress(int lengthPiece, int total_kb, int downloaded_kb);
    Q_INVOKABLE void update(int total_pieces, int currentPiece);

    QObject *_torrent;
    QVBoxLayout *layout;

public slots:
    void doRun(QString absoluteFilePath);

private:
    QVideoWidget *videoWidget;
    QMediaPlayer *player;
    QWidget *box;
    QPixmap icon;
    QLabel *volumeIcon;
    QLabel *volumeLabel;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QLabel *bitRateLabel;
    QLabel *peersLabel;
    QProgressBar *bar;
    QSlider *slider;

    bool init;
    bool control;
    bool pausedForUser;
    int volume;

    bool skip;

    int skipTo_msec;

private slots:
    void standBy();
    void playPause();
    void statusChange(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error);
    void setPositiveVolume();
    void setNegativeVolume();
    void setCurrentTime(qint64 msecs);
    void setTotalTime(qint64 msecs);
    void setSliderPosition(qint64 position);
    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();
};

#endif
