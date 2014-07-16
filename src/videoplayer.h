#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QProgressBar>
#include <QSlider>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class VideoPlayer : public QVideoWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QVideoWidget *parent=0);
    ~VideoPlayer();

    Q_INVOKABLE void downloadInfo(int bitRate, int peers);
    Q_INVOKABLE void progress(int piece_kb, int total_kb, int downloaded_kb);
    Q_INVOKABLE void update(int total_pieces, int currentPiece);

    QObject *_torrent;
    QTimer *hideControlsTimer;

    int skip_key_value;

public slots:
    void doRun(QString absoluteFilePath);
    void playPause();
    void setVolume(int volume_key_value);
    void setLoop();
    void sliderReleased();
    void showControls();

private:
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
    QSlider *sliderStartLoop;
    QSlider *sliderStopLoop;

    bool skip;
    bool paused;
    bool loop;
    int skip_player_msec;
    int volume;
    qint64 startLoop_msec;
    qint64 stopLoop_msec;

private slots:
    void standBy();
    void setCurrentTime(qint64 msecs);
    void setTotalTime(qint64 msecs);
    void setSliderPosition(qint64 position);
    void sliderPressed();
    void sliderMoved(int position);
    void sliderStartLoopMoved(int position);
    void sliderStartLoopReleased();
    void sliderStopLoopMoved(int position);
    void sliderStopLoopReleased();
    void hideControls();
    void statusChange(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error);
};

#endif
