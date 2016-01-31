#ifndef PLAYER_H
#define PLAYER_H

#include "ui.h"

#include <vlc/vlc.h> //<pig/vlc/vlc.h>

#include <QWidget>

#include <QMouseEvent>
#include <QTimer>

namespace Ui {
class Movie;
}

class Movie : public QWidget
{
    Q_OBJECT

public:
    explicit Movie(const QString* const PIG_PATH, QWidget *parent = 0);
    ~Movie();

protected:
    void mouseMoveEvent(QMouseEvent * event);

private:
    const QString* const _PIG_PATH;

    bool isSetTotalTime;
    int cacheVolume;

    libvlc_instance_t *instance;
    libvlc_media_player_t *mediaplayer;
    libvlc_media_t *media;
    libvlc_event_manager_t *events;
    //libvlc_media_stats_t *stats;

    Ui::Movie *ui;

    QTimer *t_delayControls;

private slots:
    void init_mediaplayer(QString path);
    void mediaplayer_play();
    void mediaplayer_stop();
    void mediaplayer_mute();
    int mediaplayer_set_volume(int vol);
    void mediaplayer_set_position(int pos);
    void mediaplayer_set_time(qint64 ms, bool setTotalTime = false);
    void mediaplayer_controls(bool show = true);
    void mediaplayer_update_ui();
    static void mediaplayer_callback(const struct libvlc_event_t *event, void *userData);
    void stats(int bitrate, int peers, const qint64 &kb_writen,
               const double &kb_required, const double &n_kb);
};

#endif
