#ifndef UPDATE_H
#define UPDATE_H

#include "tcpSocket.h"

#include <QObject>
#include <QProcess>
#include <QtSql>

class Update : public QObject
{
    Q_OBJECT

public:
    explicit Update(QObject *parent = 0);
    ~Update();

    QSqlDatabase db;
    QObject *_root;

public slots:
    void check();

signals:
    void forwardSIGNAL();
    void errorDatabaseSIGNAL();

private:
    TcpSocket *mSocket;

    QString hostFiles;
    QString databaseUrl;
    QString databaseHash;
    QString binaryUrl;
    QString binaryHash;
    QString newsUrl;
    QString newsHash;
    QProcess *updaterProc;

    int currentDatabaseVersion, currentBinaryVersion, currentRelease;
    bool newsAvailable, newDatabaseAvailable, newBinaryAvailable, databaseUpdated;

private slots:
    void get_version(QString host, QString url);
    void evaluate(QString version);
    void get_files();
    void integrityFile(QString path, QString file);
    void replace(QString path, QString file);
    void replace_binary_ready(int exitCode);
    void abort();
};

#endif
