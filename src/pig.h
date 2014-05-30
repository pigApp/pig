#ifndef PIG_H
#define PIG_H

#include <QObject>
#include <QApplication>
#include <QVBoxLayout>
#include <QShortcut>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QtSql>
#include <QCryptographicHash>

#include "password/pass.h"
#include "video/videoplayer.h"

class PIG : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString data READ data)
    Q_PROPERTY(QStringList dataList READ dataList)
    Q_PROPERTY(bool active READ active)
    Q_PROPERTY(int active READ active)

public:
    PIG(QObject *parent = 0);
    ~PIG();

    QWidget *window;
    QWidget *container;
    QVBoxLayout *layout;

    QStringList dataList() const { return mDataList; }
    QString data() const { return mData; }
    bool active() { return mActive; }
    int number() { return mNumber; }
    void setRootObject(QObject *root);

signals:
    void showUpdateSIGNAL();
    void showFinderSIGNAL();
    void showOutputSIGNAL();
    void listUpdatedSIGNAL();
    void noResultSIGNAL();

private:
    QSqlDatabase db;
    QString rootPath;
    QString dbPath;
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

    QStringList _list;

    Pass *mPass;
    VideoPlayer *mPlayer;
    QShortcut *Esc;

    QObject *mRoot;
    QString mData;
    QStringList mDataList;

    short int localDbVersion, localBinVersion, localRelease, pkgToDownload, row, mNumber;
    bool remoteDbAvailable, remoteBinAvailable, updateAborted, updateDownload, updateCleanAll, fixDbAborted, mActive;

private slots:
    void passManager(QString pass, bool init, bool write);

    void update();
    void updateHttp();
    void versionsReadyRead();
    void versionsFinished();
    void updateDownloadManager();
    void updateDownloadReadyRead();
    void updateDownloadFinished();
    void updateTimeOut();
    void updateRestartApp();
    void updateControl();
    void close();

    void finder();
    void finderDb(const QString inputText, QString category, QString pornstar, int offset, bool init);

    void openPlayer(QString videoID);
    void closePlayer();

    void errorDbHelper();
    void fixerDb();
    void fixerDbTimeOut();
    void getUrlReadyRead();
    void getUrlFinished();
    void fixerReadyRead();
    void fixerFinished();

    bool hashCalculation(QString md5SumPath, QString md5SumHash);
    bool copyFile(const QString& fileOrigin, const QString& destinationDir);
    bool removeFile(const QString &fileOrigin);
};
#endif
