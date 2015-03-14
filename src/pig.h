#ifndef PIG_H
#define PIG_H

#include "update.h"
#include "torrent.h"
#include "tcpSocket.h"

#include <QObject>
#include <QtSql>

class PIG : public QObject
{
    Q_OBJECT

public:
    PIG(QObject *parent = 0);
    ~PIG();

public slots:
    void quit();
    void set_root_object(QObject *root);
    void password_handler(const bool require, const QString plain, const bool check, const bool write);
    void preview_handler(const int id, const QString host, const QString url, const QString target, const bool success, const bool abort);
    void torrent_handler(const QString host, const QString url, const QString target, const int scene, const bool abort);
    void find(const QString userInput, const QString category
        , const QString pornstar, const QString quality, const QString full);

signals:
    void sig_ret_password(const bool require = false, const bool success = false);
    void sig_show_update();
    void sig_show_news(const QString binaryNews, const QString databaseNews);
    void sig_show_finder();
    void sig_ret_db(int nMovies, QStringList dataMovies);
    void sig_ret_preview(const int id, const bool success);
    void sig_show_db_err();

private:
    QObject *mRoot;
    Update *mUpdate;
    Torrent *mTorrent;
    TcpSocket *mSocket[5];

    QSqlDatabase db;

private slots:
    void update_handler();
    void start();
    void cleanup();
    void db_error();
};

#endif
