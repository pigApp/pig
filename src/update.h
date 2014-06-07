#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QtSql>
#include <QCryptographicHash>

class Update : public QObject
{
    Q_OBJECT

public:
    explicit Update(QObject *parent = 0);
    ~Update();

    QSqlDatabase db;
    QString rootPath;
    QString dbPath;

public slots:
    void init(QObject *mRoot);

signals:
    void updateCallFinderSIGNAL();
    void updateErrorDbSIGNAL();

private:
    QObject *_mRoot;
    QNetworkAccessManager *networkAccess;
    QNetworkConfigurationManager *networkAccessConfig;
    QNetworkReply *reply;
    QUrl url;
    QTimer *timeOut;
    QFile *file;
    QString fileName;

    QString remoteVersions;
    QStringList remoteVersionsList;
    QString remoteUrl;
    QStringList remoteUrlList;
    QProcess *execUpdate;
    
    short int localDbVersion, localBinVersion, localRelease, pkgToDownload;
    bool remoteDbAvailable, remoteBinAvailable, updateAborted, updateDownload, updateCleanAll;

private slots:
    void http();
    void versionsReadyRead();
    void versionsFinished();
    void downloadManager();
    void downloadReadyRead();
    void downloadFinished();
    void updateTimeOut();
    void restartApp();
    void control();
    
    bool hashCalculation(QString md5SumPath, QString md5SumHash);
    bool copyFile(const QString& fileOrigin, const QString& destinationDir);
    bool removeFile(const QString &fileOrigin);
};

#endif 
