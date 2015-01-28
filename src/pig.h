#ifndef PIG_H
#define PIG_H

#include "update.h"
#include "torrent.h"
#include "tcpSocket.h"

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QtSql>
#include <QKeyEvent>
#include <QVBoxLayout>

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent = 0);
    ~PIG();

    QVBoxLayout *layout;
    QWidget *container;

public slots:
    void set_root_object(QObject *root);
    void password_handler(const bool require, const QString plain, const bool check, const bool write);

signals:
    void sig_ret_password(const bool require = false, const bool success = false);
    void sig_show_update();
    void sig_show_news(const QString binaryNews, const QString databaseNews);
    void sig_show_finder();
    void sig_ret_db(int nFilms, QStringList dataFilms);
    void sig_ret_preview(const QString path, const int id, const bool success);
    void sig_show_db_err();

private:
    QObject *mRoot;
    Update *mUpdate;
    Torrent *mTorrent;
    TcpSocket *mSocket[5];

    QSqlDatabase db;

private slots:
    void quit();
    void update_handler();
    void start();
    void preview_handler(const QString host, const QString url, const QString path, const QString target,
                         const int id, const bool success, const bool abort);
    void torrent_handler(const QString url, const int scene, const bool abort);
    void find(const QString userInput, const QString pornstar, const QString category,
              const QString quality, const QString full);
    void cleanup();
    void db_err();
};

#endif
