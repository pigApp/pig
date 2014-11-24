#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QProgressBar>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>

class VideoPlayer : public QVideoWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QVideoWidget *parent=0, const QString *absoluteFilePath=NULL);
    ~VideoPlayer();

    QObject *_torrent;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

public slots:
    void download_Information(int bitRate, int peers);
    void progress(qint64 total_kb, qint64 downloaded_kb, int downloadedSkip_mb);
    void update_player();

signals:
    void signal_file_ready(const QString absoluteFilePath, const bool sandbox, const bool fileReady, const bool close);
    void signal_close_player(const QString absoluteFilePath, const bool sandbox, const bool fileReady, const bool close);
    void signal_quit();

private:
    QMediaPlayer *player;
    QWidget *box;
    QLabel *pauseLabel, *bufferLabel;
    QLabel *startLoopTimeLabel, *stopLoopTimeLabel;
    QLabel *currentTimeLabel, *totalTimeLabel;
    QLabel *volumeIcon, *volumeLabel;
    QLabel *bitRateLabel, *peersLabel;
    QSlider *slider, *sliderStartLoop, *sliderStopLoop;
    QProgressBar *progressBar, *skipBar;
    QTimer *loadingStateDelay, *hideControlsDelay;
    qint64 readyToRead_msec, update_msec, startLoop_msec, stopLoop_msec;

    bool onSandbox, buffering, skip, paused, loop;
    int skip_key_value, volume, screenWidth, screenHeight;

private slots:
    void play_pause_stop();
    void onBuffering();
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
    void set_volume(int volume_key_value);
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
