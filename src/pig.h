#ifndef PIG_H
#define PIG_H

#include "update.h"
#include "torrent.h"
#include "videoplayer.h"
#include "tcpSocket.h"

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QtSql>
#include <QKeyEvent>
#include <QVBoxLayout>

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent=0);
    ~PIG();

    QVBoxLayout *layout;
    QWidget *container;

public slots:
    void set_root_object(QObject *root);
    void password_handler(const QString pass, const bool require, const bool check, const bool write);

signals:
    void signal_require_password();
    void signal_success_password();
    void signal_fail_password();
    void signal_show_update();
    void signal_show_welcome();
    void signal_show_news(const QString binaryNews, const QString databaseNews);
    void signal_show_finder();
    void signal_no_result();
    void signal_show_output(int nFilms, QStringList dataFilms);
    void signal_success_update_data(int nFilms, QStringList dataFilms);
    void signal_success_preview(const QString path, const int id);
    void signal_fail_preview(const int id);
    void signal_checking_file();
    void signal_file_ready();
    void signal_hide_torrent_information();
    void signal_show_errorDatabase();

private:
    QObject *mRoot;
    Update *mUpdate;
    Torrent *mTorrent;
    VideoPlayer *mPlayer;
    TcpSocket *mSocket[5];

    QSqlDatabase db;

private slots:
    void update_handler();
    void start_pig();
    void find(const QString inputText, const QString pornstar, const QString category,
              const QString quality, const QString full, const int offset, const bool init);
    void preview_handler(const QString host, const QString url, const QString path, const QString target,
                         const int id, const bool success, const bool fail, const bool abort);
    void torrent_handler(const QString magnet, const int scene, const bool abort);
    void player_handler(const QString absoluteFilePath, const bool sandbox, const bool fileReady, const bool close);
    void cleanUp();
    void error_database();
    void quit();
};

#endif
