#ifndef PLAYER_H
#define PLAYER_H

#include "ui.h"

#include <vlc/vlc.h> //<pig/vlc/vlc.h>

#include <QWidget>

namespace Ui {
class Movie;
}

class Movie : public QWidget
{
    Q_OBJECT

public:
    explicit Movie(const QString* const PIG_PATH, QWidget *parent = 0);
    ~Movie();
    
private:
    const QString* const _PIG_PATH;

    bool isInitPlayer;

    libvlc_instance_t *instance;
    libvlc_media_player_t *mediaplayer;
    libvlc_media_t *media;
    //libvlc_media_stats_t *stats;

    Ui::Movie *ui;

private slots:
    void init_mediaplayer(QString path);
    void stats(int bitrate, int peers, const qint64 &kb_writen,
               const double &kb_required, const double &n_kb);
};

#endif
