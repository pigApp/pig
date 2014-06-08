#ifndef UPDATETEST_H
#define UPDATETEST_H

#include <QObject>
#include <QtSql>

#include "tcpSocket.h"

class UpdateTest : public QObject
{
    Q_OBJECT

public:
    explicit UpdateTest(QObject *parent = 0);

    QSqlDatabase db;

public slots:
    void doCheck(QObject *mRoot);

signals:
    void updateCallFinder();
    void updateFail();
    void updateErrorDb();

private:
    QObject *_mRoot;
    QString url;
    QString dbMD5;
    QString binaryMD5;
    //QProcess *execUpdate;

    TcpSocket mSocket;

    int currentDBVersion, currentBinaryVersion;
    bool newDBAvailable, newBinaryAvailable;

private slots:
    void doCheck(QObject *mRoot);
    void getLastVersion();
    void evaluate(QString lastVersion);
    void integrityFile(QString path, QString fileName);
    void replaceFile(QString path, QString fileName);



    bool hashCalculation(QString md5SumPath, QString md5SumHash);
    bool copyFile(const QString& fileOrigin, const QString& destinationDir);
    bool removeFile(const QString &fileOrigin);

};

#endif
