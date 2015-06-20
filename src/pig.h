#ifndef PIG_H
#define PIG_H

#include "update.h"
#include "tcpSocket.h"
#include "torrent.h"

#include <QObject>
#include <QtSql>

class PIG : public QObject
{
    Q_OBJECT

public:
    PIG(QObject *parent = 0);
    ~PIG();

public slots:
    void set_root_object(QObject *root);
    void password(const bool require, const QString plain
        , const bool check, const bool write);
    void preview(const int id, const QString host, const QString url
        , const QString target
        , const bool ready, const bool success, const bool error, const bool abort);
    void torrent(const QString host, const QString url, const QString target
        , const int scene, const bool abort);
    void find(const QString userInput, const QString category
        , const QString pornstar, const QString quality, const QString full);
    void quit();

signals:
    void sig_show_update();
    void sig_show_news(const QString binaryNews, const QString databaseNews);
    void sig_show_finder();
    void sig_show_db_err();
    void sig_ret_password(const bool require = false, const bool success = false);
    void sig_ret_db(int nMovies, QStringList dataMovies);
    void sig_ret_stream(const int id, const bool ready, const bool success
        , const bool error);

private:
    QObject *mRoot;
    Update *mUpdate;
    TcpSocket *mSocket[5];
    Torrent *mTorrent;

    QSqlDatabase db;

private slots:
    void update();
    void start();
    void cleanup();
    void db_error();
};

#endif
