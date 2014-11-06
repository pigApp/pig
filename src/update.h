#ifndef UPDATE_H
#define UPDATE_H

#include "tcpSocket.h"

#include <QObject>
#include <QtSql>
#include <QProcess>

class Update : public QObject
{
    Q_OBJECT

public:
    explicit Update(QObject *parent = 0);
    ~Update();

    QObject *_root;
    QSqlDatabase *db;

public slots:
    void start();

signals:
    void forward_signal();
    void fail_database_signal();

private:
    TcpSocket *mSocket;

    QString hostFiles;
    QString databaseUrl;
    QString databaseHash;
    QString binaryUrl;
    QString binaryHash;
    QString newsUrl;
    QString newsHash;
    QProcess *suManagerProc;
    QProcess *updateProc;

    bool newsAvailable, newDatabaseAvailable, newBinaryAvailable, databaseUpdated;
    int currentDatabaseVersion, currentBinaryVersion, currentRelease;

private slots:
    void get_version(const QString * const host, const QString * const url);
    void evaluate(const QString version);
    void get_files();
    void integrityFile(const QString path, const QString file);
    void replace(QString *path, QString *file);
    void replace_binary_success(int exitCode);
    void error();
};

#endif
