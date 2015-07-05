#ifndef UPDATE_H
#define UPDATE_H

#include "tcpsocket.h"
#include "su.h"

#include <QObject>
#include <QtSql>

class Update : public QObject
{
    Q_OBJECT

public:
    explicit Update(const QString *PIG_PATH, QSqlDatabase *db, QObject *parent = 0);
    ~Update();

public slots:
    void start();

signals:
    void finished();
    void sig_error();

private:
    TcpSocket *socket;
    Su *su;

    const QString *_PIG_PATH;
    QSqlDatabase *_db;

    QString host;
    QStringList urls, sums, targets;

    bool hasNewBin, hasNewDb, hasNewLib;
    int bin, rel, db, lib;
    int updatedBin, updatedRel, updatedDb, updatedLib;

private slots:
    void get();
    void check_versions(const QString *const str);
    void user_confirmation();
    void unzip_files(const QStringList *const files);
    void update_files();
    void check_exit(int exitCode);
    void error();
};

#endif
