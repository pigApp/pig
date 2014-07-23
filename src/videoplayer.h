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
#include <QKeyEvent>
#include <QMouseEvent>

class VideoPlayer : public QVideoWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QVideoWidget *parent = 0);
    ~VideoPlayer();

    Q_INVOKABLE void downloadInfo(int bitRate, int peers);
    Q_INVOKABLE void progress(int piece_kb, int total_kb, int downloaded_kb);
    Q_INVOKABLE void update(int total_pieces, int currentPiece);

    QObject *_pig;
    QObject *_torrent;
    QTimer *hideControlsTimer;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

public slots:
    void doRun(QString absoluteFilePath);

private:
    QMediaPlayer *player;
    QWidget *box;
    QPixmap volumePixmap;
    QPixmap loopPixmap;
    QLabel *volumeIcon;
    QLabel *volumeLabel;
    QLabel *pauseLabel;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QLabel *bitRateLabel;
    QLabel *peersLabel;
    QLabel *loopIcon;
    QLabel *loopLabel;
    QLabel *loopTimeLabel;
    QProgressBar *bar;
    QSlider *slider;
    QSlider *sliderStartLoop;
    QSlider *sliderStopLoop;

    bool skip;
    bool paused;
    bool loop;
    int readyToRead_msec;
    int skip_key_value;
    int skip_player_msec;
    int volume;
    int screenWidth;
    int screenHeight;
    qint64 startLoop_msec;
    qint64 stopLoop_msec;

private slots:
    void playPause();
    void standBy();
    void setVolume(int volume_key_value);
    void setLoop();
    void sliderStartLoopPressed();
    void sliderStartLoopMoved(int position);
    void sliderStartLoopReleased();
    void sliderStopLoopPressed();
    void sliderStopLoopMoved(int position);
    void sliderStopLoopReleased();
    void setSliderLoopTime(int msec);
    void setCurrentTime(qint64 msecs);
    void setTotalTime(qint64 msecs);
    void setSliderPosition(qint64 position);
    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();
    void showControls();
    void hideControls();
    void statusChange(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error);
};

#endif
