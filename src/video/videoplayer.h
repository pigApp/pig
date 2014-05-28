#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <src/video/QtAV/WidgetRenderer.h>
#include <src/video/QtAV/AudioOutput.h>
#include <src/video/QtAV/AVPlayer.h>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    void setAudioOutput(QtAV::AudioOutput* ao);

public slots:
    void open(const QString& file);

private slots:
    void play_pause();
    void stop();
    void onStartPlay();
    void onPositionChange(qint64 position);
    void onPositionSliderChange();
    void enable_disable_audio();
    void setVolume();

private:
    QtAV::AVPlayer mediaPlayer;
    QtAV::WidgetRenderer renderer;

    QPushButton *play_pause_Button;
    QPushButton *stopButton;
    QSlider *slider;
    QPushButton *volumeButton;
    QSlider *volumeSlider;

    qint64 currentPosition;
};
#endif
