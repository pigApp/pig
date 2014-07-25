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

    Q_INVOKABLE void download_Information(int bitRate, int peers);
    Q_INVOKABLE void progress(int piece_kb, int total_kb, int downloaded_kb);
    Q_INVOKABLE void update(int total_pieces, int currentPiece);

    QObject *_pig;
    QObject *_torrent;
    QTimer *hideControlsTimer;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

public slots:
    bool sandbox(QString absoluteFilePath);
    void start(QString absoluteFilePath);

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
    void play_pause();
    void standBy();
    void set_volume(int volume_key_value);
    void set_loop();
    void slider_startLoop_pressed();
    void slider_startLoop_moved(int position);
    void slider_startLoop_released();
    void slider_stopLoop_pressed();
    void slider_stopLoop_moved(int position);
    void slider_stopLoop_released();
    void set_sliderLoop_time(int msec);
    void set_currentTime(qint64 msecs);
    void set_totalTime(qint64 msecs);
    void set_slider_position(qint64 position);
    void slider_pressed();
    void slider_moved(int position);
    void slider_released();
    void show_controls();
    void hide_controls();
    void status_change(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error);
};

#endif
