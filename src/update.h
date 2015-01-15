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
    explicit Update(QObject *parent = 0);
    ~Update();

    QObject **_root;
    QSqlDatabase *db;

public slots:
    void start();

signals:
    void signal_continue();
    void signal_fail_database();

private:
    TcpSocket *mSocket;
    Su *mSu;

    QString host;
    QStringList urls, sums;

    bool binaryAvailable, databaseAvailable, libraryAvailable;
    int binary, release, database, library;
    int newBinary, newRelease, newDatabase, newLibrary;

private slots:
    void get(const QString *const host, const QStringList *const urls, const QString request);
    void check_versions(const QString *const str);
    void user_confirmation();
    void unzip_files(const QString * const path, const QStringList * const files);
    void update_files();
    void check_exit(int exitCode);
    void error();
};

#endif
