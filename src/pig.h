#ifndef PIG_H
#define PIG_H

#include "update.h"
#include "torrent.h"
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
    void signal_ret_password(const bool require=false, const bool success=false);
    void signal_show_update();
    void signal_show_news(const QString binaryNews, const QString databaseNews);
    void signal_show_finder();
    void signal_ret_db(int blockFilms, QStringList dataFilms, bool updateData);
    void signal_ret__preview(const QString path, const int id, const bool success);
    void signal_show_errorDatabase();

private:
    QObject *mRoot;
    Update *mUpdate;
    Torrent *mTorrent;
    TcpSocket *mSocket[5];

    QSqlDatabase db;

private slots:
    void update_handler();
    void start_pig();
    void find(const QString inputText, const QString pornstar, const QString category,
              const QString quality, const QString full, const int offset, const bool init);
    void preview_handler(const QString host, const QString url, const QString path, const QString target,
                         const int id, const bool success, const bool abort);
    void torrent_handler(const QString url, const int scene, const bool abort);
    void cleanup();
    void error_database();
    void quit();
};

#endif
