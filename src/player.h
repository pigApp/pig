#ifndef PLAYER_H
#define PLAYER_H

#include "ui.h"

#include <vlc/vlc.h> //<pig/vlc/vlc.h>

#include <QWidget>

#include <QObject>

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(const QString *path = NULL, QWidget *parent = 0);
    ~Player();

    libvlc_media_player_t *mediaplayer;
    


public slots:
   void set_bitrate(QString bitrate) { m_bitrate = bitrate; } 
   void set_peers(QString peers) { m_peers = peers; } 
   void set_kb_required(double kb_required) { m_kb_required = kb_required; } 
   void set_n_kb(double n_kb) { m_n_kb = n_kb; } 
   void set_kb_writen(qint64 kb_writen) { m_kb_writen = kb_writen; } 

private:
    QString m_bitrate;
    QString m_peers;
    double m_kb_required;
    double m_n_kb;
    qint64 m_kb_writen;
    
    Ui::Player *ui;

    libvlc_instance_t *instance;
    libvlc_media_t *media;
    //libvlc_media_stats_t *stats;

private slots:
    void stats();
};

#endif
