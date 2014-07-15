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
    Q_INVOKABLE void progress(int piece_kb, int total_kb, int downloaded_kb);
    Q_INVOKABLE void update(int total_pieces, int currentPiece);

    QObject *_torrent;
    QVBoxLayout *layout;

    int skip_key_value;

public slots:
    void doRun(QString absoluteFilePath);
    void playPause();
    void setVolume(int volume_key_value);
    void sliderReleased();

private:
    QVideoWidget *videoWidget;
    QMediaPlayer *player;
    QWidget *box;
    QPixmap icon;
    QLabel *volumeIcon;
    QLabel *volumeLabel;
    QLabel *pauseLabel;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QLabel *bitRateLabel;
    QLabel *peersLabel;
    QProgressBar *bar;
    QSlider *slider;

    bool skip;
    bool paused;
    int skip_player_msec;
    int volume;


private slots:
    void standBy();
    void statusChange(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error);
    void setCurrentTime(qint64 msecs);
    void setTotalTime(qint64 msecs);
    void setSliderPosition(qint64 position);
    void sliderPressed();
    void sliderMoved(int position);
};

#endif
