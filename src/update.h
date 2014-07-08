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
    void doCheck();

signals:
    void forward();
    void errorDb();

private:
    TcpSocket mSocket;

    QString hostFiles;
    QString databaseUrl;
    QString databaseHash;
    QString binaryUrl;
    QString binaryHash;
    QString newsUrl;
    QString newsHash;
    QProcess *updaterProc;

    bool unaVez;//

    int currentDatabaseVersion, currentBinaryVersion, currentRelease;
    bool newsAvailable, newDatabaseAvailable, newBinaryAvailable, databaseUpdated;

private slots:
    void getVersion(QString host, QString url);
    void evaluate(QString version);
    void getFiles();
    void integrityFile(QString path, QString file);
    void replace(QString path, QString file);
    void replaceBinaryReady(int exitCode);
    void error();
};

#endif
