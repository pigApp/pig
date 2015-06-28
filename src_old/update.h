#ifndef UPDATE_H
#define UPDATE_H

#include "tcpSocket.h"
#include "su.h"

#include <QObject>
#include <QtSql>

class Update : public QObject
{
    Q_OBJECT

public:
    explicit Update(QObject *parent = 0, QObject **root = NULL
        , QSqlDatabase *db = NULL);
    ~Update();

signals:
    void sig_continue();
    void sig_fail_database();

private:
    QObject **_root;
    TcpSocket *mSocket;
    Su *mSu;

    QSqlDatabase *_db;
    QString host;
    QStringList urls, sums, targets;

    bool binaryAvailable, databaseAvailable, libraryAvailable;
    int binary, release, database, library;
    int newBinary, newRelease, newDatabase, newLibrary;

private slots:
    void start();
    void get(const QString *const host, const QStringList *const urls
        , const QStringList *const targets);
    void check_versions(const QString *const str);
    void user_confirmation();
    void unzip_files(const QString *const tmp, const QStringList *const files);
    void update_files();
    void check_exit(int exitCode);
    void error();
};

#endif
