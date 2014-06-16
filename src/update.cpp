#include <QDebug>

#include "update.h"

Update::Update(QObject *parent) : QObject(parent)
{
   /*
   #ifdef _WIN32
        rootPath = "C:/pig/";
        dbPath = rootPath+".pig/db.sqlite";
    #else
        dbPath = QDir::homePath()+"/.pig/db.sqlite";
    #endif
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    */
     rootPath = "C:/pig/";
}

Update::~Update()
{
    delete networkAccess;
    delete networkAccessConfig;
}

void Update::init(QObject *obj)
{
    _root = obj;

    networkAccess = new QNetworkAccessManager();
    networkAccessConfig = new QNetworkConfigurationManager();
    networkAccess->setConfiguration(networkAccessConfig->defaultConfiguration());

    _root->setProperty("status", "SEEK UPDATE");
    _root->setProperty("showSpinner", true);
    if (db.open()) {
        QSqlQuery qry;
        qry.prepare("SELECT DbVersion, BinVersion FROM PigData");
        if (!qry.exec()) {
            db.close();
            emit updateErrorDb();
        } else {
            qry.next();
            localDbVersion  = qry.value(0).toInt();
            localBinVersion = qry.value(1).toInt();
            db.close();

            remoteBinAvailable = false;
            remoteDbAvailable = false;
            updateDownload = false;
            updateAborted = false;

            http();
        }
    } else {
      emit updateErrorDb();
    }
}

void Update::http()
{
    if (!updateDownload) {
        url = "http://gamenetworkmanager.herokuapp.com/update";
        reply = networkAccess->get(QNetworkRequest(url));
        connect(reply, SIGNAL(readyRead()), this, SLOT(versionsReadyRead()));
        connect(reply, SIGNAL(finished()), this, SLOT(versionsFinished()));

        timeOut = new QTimer();
        timeOut->setSingleShot(true);
        connect(timeOut, SIGNAL(timeout()), this, SLOT(updateTimeOut()));
        timeOut->start(15000);
    } else {
        _root->setProperty("status", "UPDATING");
        _root->setProperty("showSpinner", true);
        _root->setProperty("showDecisionButton", true);

        #ifdef _WIN32
            file = new QFile("C:/tmp/"+fileName);
        #else
            file = new QFile("/tmp/"+fileName);
        #endif
        if (!file->open(QIODevice::WriteOnly)) {
            _root->setProperty("showSpinner", false);
            _root->setProperty("showDecisionButton", false);
            #ifdef _WIN32
                _root->setProperty("status", "PERMISSION DENIED");
                _root->setProperty("statusInformation", "restart the application with administrator rights");
            #else
                _root->setProperty("status", "FAILED TO UPDATE THE DATABASE");
                _root->setProperty("statusInformation", "TRY LTER");
                QTimer::singleShot(3000, this, SLOT(finder()));
            #endif
        } else {
            reply = networkAccess->get(QNetworkRequest(url));
            connect(reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
            connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));

            timeOut->start(25000);
            connect(timeOut, SIGNAL(timeout()), this, SLOT(updateTimeOut()));
        }
    }
}

void Update::updateTimeOut()
{
    updateAborted = true;
    emit updateCallFinder();
}

void Update::versionsReadyRead()
{
    remoteVersions = reply->readAll();
}

void Update::versionsFinished()
{
    if (updateAborted) {
        timeOut->stop();
        reply->deleteLater();
        updateAborted = false;
        _root->setProperty("showSpinner", false);
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        pkgToDownload = 0;
        timeOut->stop();
        remoteVersionsList = remoteVersions.split(",");

        if (remoteVersionsList[0].toInt() > localDbVersion) {
            remoteDbAvailable = true;
            ++pkgToDownload;
        }
        if (remoteVersionsList[6].toInt() > localBinVersion) {
            remoteBinAvailable = true;
            ++pkgToDownload;
        }

        QTimer::singleShot(2000, this, SLOT(downloadManager()));
    } else {
        timeOut->stop();
        reply->close();
        reply = 0;
        _root->setProperty("showSpinner", false);
        emit updateCallFinder();
        //QTimer::singleShot(2000, this, SLOT(finder()));
    }
}

void Update::downloadManager()
{
    if (remoteDbAvailable) {
        updateDownload = true;
        reply->close();
        reply = 0;
        fileName = "db.sqlite";
        url = remoteVersionsList[1];
        _root->setProperty("status", "UPDATE AVAILABLE");
        _root->setProperty("showSpinner", false);
        _root->setProperty("showDecisionButton", true);
        if (remoteBinAvailable) {
            _root->setProperty("statusInformation", "THERE IS A NEW VERSION OF DATABASE AND BINARY PACKAGE");
        } else {
            _root->setProperty("statusInformation", "THERE IS A NEW VERSION OF DATABASE");
        }
    } else if(remoteBinAvailable) {
        updateDownload = true;
        reply->close();
        reply = 0;
        #ifdef _WIN32
            fileName = "PIG.exe";
            url = remoteVersionsList[7];
        #else
            fileName = "PIG";
            url = remoteVersionsList[7];
        #endif
        _root->setProperty("status", "UPDATE AVAILABLE");
        _root->setProperty("showSpinner", false);
        _root->setProperty("showDecisionButton", true);
        _root->setProperty("statusInformation", "THERE IS A NEW VERSION OF BINARY PACKAGE");
    } else {
      reply->close();
      reply = 0;
      _root->setProperty("showSpinner", false);
      emit updateCallFinder();
    }
}

void Update::downloadReadyRead()
{
    file->write(reply->readAll());
}

void Update::downloadFinished()
{
    static short int pkgToDownloadControl = 0;
    QString hashDb = remoteVersionsList[2];
    #ifdef _WIN32
        QString dbPath  = "C:/pig/.pig/db.sqlite";
        QString fileOriginDb = "C:/tmp/db.sqlite";
        QString fileTarget = rootPath+".pig/db.sqlite";
        QString md5SumPathDb = fileOriginDb;
        QString fileOriginBin = "C:/tmp/PIG.exe";
        QString hashBin = remoteVersionsList[8];
        QString md5SumPathBin = "C:/tmp/PIG.exe";
    #else
        QString dbPath  = QDir::homePath()+"/.pig/db.sqlite";
        QString fileOriginDb = "/tmp/db.sqlite";
        QString fileTarget = QDir::homePath()+"/.pig/db.sqlite";
        QString md5SumPathDb = fileOriginDb;
        QString fileOriginBin = "/tmp/PIG";
        QString hashBin = remoteVersionsList[8];
        QString md5SumPathBin = "/tmp/PIG";
    #endif
    if (updateAborted) {
        timeOut->stop();
        updateAborted = false;
        if (file)
            file->close();
        reply->deleteLater();
        _root->setProperty("showSpinner", false);
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        timeOut->stop();
        file->flush();
        file->close();
        ++pkgToDownloadControl;

        if (pkgToDownload < 2) {
            reply->close();
            reply = 0;
            _root->setProperty("showSpinner", false);
            if (!remoteBinAvailable) {
                QFileInfo file(dbPath);
                if (file.isFile())
                    removeFile(dbPath);
                if (hashCalculation(md5SumPathDb, hashDb)) {
                        if (!copyFile(fileOriginDb, fileTarget)) {
                            #ifdef _WIN32
                                _root->setProperty("status", "PERMISSION DENIED");
                                _root->setProperty("statusInformation", "RESTART THE APPLICATION WITH ADMINITRATOR RIGHTS");
                            #else
                                _root->setProperty("status", "FAILED TO UPDATE THE DATABASE");
                                _root->setProperty("statusInformation", "TRY LATER");
                                emit updateCallFinder();
                                //QTimer::singleShot(3000, this, SLOT(finder()));
                            #endif
                        } else {
                            removeFile(fileOriginDb);
                            _root->setProperty("status", "UPDATED DATABASE");
                            emit updateCallFinder();
                            //QTimer::singleShot(3000, this, SLOT(finder()));
                        }
                } else {
                   removeFile(fileOriginDb);
                   emit updateCallFinder();
                }
            } else {
                if (hashCalculation(md5SumPathBin, hashBin)) {
                    #ifdef _WIN32
                        _root->setProperty("status", "UPDATED");
                        _root->setProperty("statusInformation", "RESTART THE TO MAKE THE CHANGES EFFECTIVE");
                        _root->setProperty("restart", true);
                    #else
                        _root->setProperty("status", "UPDATED");
                        _root->setProperty("statusInformation", "AUTHORIZATION IS REQUIRED");
                        _root->setProperty("authorize", true);
                    #endif
                } else {
                    removeFile(fileOriginBin);
                    emit updateCallFinder();
                }
           }
        } else {
            if (pkgToDownloadControl != pkgToDownload) {
                if (hashCalculation(md5SumPathDb, hashDb)) {
                    remoteDbAvailable = false;
                    #ifdef _WIN32
                        fileName = "PIG.exe";
                        url = remoteVersionsList[7];
                    #else
                        fileName = "PIG";
                        url = remoteVersionsList[7];
                    #endif

                    http();
                } else {
                    removeFile(fileOriginDb);
                    _root->setProperty("showSpinner", false);
                    emit updateCallFinder();
                }
            } else {
                reply->close();
                reply = 0;
                updateCleanAll = true;
                _root->setProperty("showSpinner", false);
                if (hashCalculation(md5SumPathBin, hashBin)) {
                    QFileInfo file(dbPath);
                    if (file.isFile())
                        removeFile(dbPath);
                    if (!copyFile(fileOriginDb, fileTarget)) {
                        #ifdef _WIN32
                            _root->setProperty("status", "PERMISSION DENIED");
                            _root->setProperty("statusInformation", "RESTART THE APPLICATION WITH ADMINITRATOR RIGHTS");
                        #else
                            _root->setProperty("status", "UPDATE FAILED");
                            _root->setProperty("statusInformation", "TRY LATER");
                            emit updateCallFinder();
                            //QTimer::singleShot(3000, this, SLOT(finder()));
                        #endif
                    } else {
                        removeFile(fileOriginDb);
                        #ifdef _WIN32
                            _root->setProperty("status", "UPDATED");
                            _root->setProperty("statusInformation", "RESTART THE TO MAKE THE CHANGES EFFECTIVE");
                            _root->setProperty("restart", true);
                        #else
                            _root->setProperty("status", "UPDATED");
                            _root->setProperty("statusInformation", "AUTHORIZATION IS REQUIRED");
                            _root->setProperty("authorize", true);
                        #endif
                    }
                } else {
                    removeFile(fileOriginDb);
                    removeFile(fileOriginBin);
                    emit updateCallFinder();
                }
            }
        }

    }else {
        timeOut->stop();
        if (file)
            file->close();
        reply->close();
        reply = 0;
        _root->setProperty("showSpinner", false);
        emit updateCallFinder();
    }
}

void Update::restartApp()
{
    #ifdef _WIN32
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        QString pathUpdateBin = rootPath+"bin/PXupdate.exe";
        QString updateBinArgument = " C:/tmp/PIG.exe "+rootPath+"bin/PIG.exe";
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (!CreateProcess((TCHAR*)(pathUpdateBin.utf16()), (TCHAR*)(updateBinArgument.utf16()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            _root->setProperty("status", "PERMISSION DENIED");
            _root->setProperty("statusInformation", "RESTART THE APPLICATION WITH ADMINITRATOR RIGHTS");
        } else {
            control();
        }
    #else
        QString userName = getenv("USER");
        QString updatePathArgs = "gksu PXupdate "+userName+" &";
        execUpdate = new QProcess(this);
        connect(execUpdate, SIGNAL(started()), this, SLOT(control()));
        execUpdate->start("/bin/bash", QStringList() << "-c" << updatePathArgs);
    #endif
}

void Update::control()
{
    #ifdef _WIN32
        if (remoteBinAvailable && !remoteDbAvailable) {
            if (db.open()) {
                QSqlQuery qry;
                qry.prepare("UPDATE PigData SET BinVersion='"+remoteVersionsList[6]+"'");
                qry.exec();
                db.close();
            }
        }
        exit(0);
    #else
        if (execUpdate->state()==QProcess::Running && !execUpdate->error()==QProcess::FailedToStart) {
            if (remoteBinAvailable && !remoteDbAvailable) {
                if (db.open()) {
                    QSqlQuery qry;
                    qry.prepare("UPDATE PigData SET BinVersion='"+remoteVersionsList[6]+"'");
                    qry.exec();
                    db.close();
                }
            }
            exit(0);
        } else {
            _root->setProperty("status", "UPDATE FAILED");
            _root->setProperty("statusInformation", "TRY LATER");
            emit updateCallFinder();
            //QTimer::singleShot(3000, this, SLOT(finder()));
        }
    #endif
}

// Helpers
bool Update::hashCalculation(QString md5SumPath, QString md5SumHash)
{
    QFile file(md5SumPath);
    file.open(QIODevice::ReadOnly);
    QByteArray hashData = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);

    if (hashData.toHex() == md5SumHash) {
        return true;
    } else {
        if (updateCleanAll) {
            updateCleanAll = false;
            return false;
        } else {
            return false;
        }
    }
    return false;
}

bool Update::copyFile(const QString& fileOrigin, const QString& fileTarget)
{
    bool result = QFile::copy(fileOrigin, fileTarget);
    return result;
}
bool Update::removeFile(const QString &fileOrigin)
{
    bool result = QFile::remove(fileOrigin);
    return result;
}
