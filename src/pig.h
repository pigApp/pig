#ifndef PIG_H
#define PIG_H

#include "tcpSocket.h"
#include "password.h"
#include "update.h"
#include "torrent.h"
#include "videoplayer.h"

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
    void password_handler(const QString pass, const bool write);

signals:
    void require_password_signal();
    void success_password_signal();
    void fail_password_signal();
    void show_update_signal();
    void show_welcome_signal();
    void show_news_signal(const QString binaryNews, const QString databaseNews);
    void show_finder_signal();
    void no_result_signal();
    void show_output_signal(int n, QStringList list);
    void success_update_list_signal(int n, QStringList list);
    void success_preview_signal(const QString path, const QString file, const int id);
    void fail_preview_signal(const int id);
    void checking_file_signal();
    void file_ready_signal();
    void hide_torrent_information_signal();
    void show_errorDatabase_signal();

private:
    QObject *mRoot;
    TcpSocket **mSocket;
    Password *mPassword;
    Update *mUpdate;
    Torrent *mTorrent;
    VideoPlayer *mPlayer;

    QSqlDatabase db;

private slots:
    void update_handler();
    void start_pig();
    void find(const QString inputText, const QString pornstar, const QString category, const QString quality, const QString full, const int offset, const bool init);
    void preview_handler(const QString host, const QString url, const QString path, const QString file, const int id, const bool success, const bool fail, const bool abort);
    void torrent_handler(const QString magnet, const int scene, const bool stop);
    void player_handler(const QString absoluteFilePath, const bool sandbox, const bool fileReady, const bool close);
    void cleanUp();
    void error_database();
    void quit();
};

#endif
