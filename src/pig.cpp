#ifdef _WIN32
#include <windows.h>
#endif

#include "pig.h"

PIG::PIG(QObject *parent)
    : QObject(parent), mRoot(0)
{
    window = new QWidget;
    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    layout->setSpacing(0);
    window->setLayout(layout);

    #ifdef _WIN32
        rootPath = "C:/pig/";
        dbPath = rootPath+".pig/db.sqlite";
    #else
        dbPath = QDir::homePath()+"/.pig/db.sqlite";
    #endif
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    networkAccess = new QNetworkAccessManager();
    networkAccessConfig = new QNetworkConfigurationManager();
    networkAccess->setConfiguration(networkAccessConfig->defaultConfiguration());

    Esc = new QShortcut(window);
    Esc->setKey(Qt::Key_Escape);
    Esc->setEnabled(false);
    connect(Esc, SIGNAL(activated()), this, SLOT(closePlayer()));

    startTimer(1000);
}

PIG::~PIG()
{
}

// Password
void PIG::passManager(QString plain, bool init, bool write)
{
    if (init) { // Comprueba si se usa password.
        if (mPass->requirePassCheck()) {
            if (mRoot) mActive = true; mRoot->setProperty("requirePass", mActive); // Si se usa password, activa el input para escribirlo y desactiva setear password.
            if (mRoot) mActive = true; mRoot->setProperty("showAskPass", mActive);
            init = false;
        } else {
            QStringList args = qApp->arguments(); // Si no se usa password, inicia.
            if (args.last() == "WITHOUT_UPDATE")
                finder();
            else
                QTimer::singleShot(1000, this, SLOT(update()));
            init = false;
        }
    } else if (!init && !write) { // Llama a checkPass y le pasa el password ingresado para comprobarlo.
        if (mPass->checkPass(plain)) { // Si el password coincide, inicia.
            QStringList args = qApp->arguments();
            if (args.last() == "WITHOUT_UPDATE") {
                finder();
            } else {
                if (mRoot) mActive = false; mRoot->setProperty("showAskPass", mActive);
                QTimer::singleShot(350, this, SLOT(update()));
            }
        } else {
           if (mRoot) mActive = true; mRoot->setProperty("failPass", mActive); // Si el password no coincide envia un bool true para que se muestre el mensaje, la contraseña no coincide.
        }
    } else if (write) { // Para setear el password con la aplicacion ya iniciada.
        if (mPass->writePass(plain)) {
            if (mRoot) mActive = true; mRoot->setProperty("okPass", mActive); // Se escribio.
        } else {
            if (mRoot) mActive = true; mRoot->setProperty("failPass", mActive); // No se escribio.
        }
    }
}

// Update
void PIG::update()
{
    QFileInfo file(dbPath);
    if (!file.isFile()) {
        errorDbHelper();
    } else {
        if (mRoot) mData = "searching update"; mRoot->setProperty("status", mData);
        if (mRoot) mActive = true; mRoot->setProperty("showSpinner", mActive);
        if (db.open()) {
            QSqlQuery qry;
            qry.prepare("SELECT DbVersion, BinVersion FROM PxData");
            if (!qry.exec()) {
                db.close();
                errorDbHelper();
            } else {
                qry.next();
                localDbVersion  = qry.value(0).toInt();
                localBinVersion = qry.value(1).toInt();
                db.close();

                remoteBinAvailable = false;
                remoteDbAvailable = false;
                updateDownload = false;
                updateAborted = false;

                updateHttp();
            }
        } else {
          errorDbHelper();
        }
    }
}

void PIG::updateHttp()
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
        if (mRoot) mData = "updating"; mRoot->setProperty("status", mData);
        if (mRoot) mActive = true;  mRoot->setProperty("showSpinner", mActive);
        if (mRoot) mActive = false; mRoot->setProperty("showDecisionButton", mActive);
        #ifdef _WIN32
            file = new QFile("C:/tmp/"+fileName);
        #else
            file = new QFile("/tmp/"+fileName);
        #endif
        if (!file->open(QIODevice::WriteOnly)) {
            if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
            if (mRoot) mActive = false; mRoot->setProperty("showDecisionButton", mActive);
            #ifdef _WIN32
                if (mRoot) mData = "PERMISSION DENIED"; mRoot->setProperty("status", mData);
                if (mRoot) mData = "restart the application with administrator rights"; mRoot->setProperty("statusInformation", mData);
            #else
                if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
                if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
                QTimer::singleShot(3000, this, SLOT(finder()));
            #endif
        } else {
            reply = networkAccess->get(QNetworkRequest(url));
            connect(reply, SIGNAL(readyRead()), this, SLOT(updateDownloadReadyRead()));
            connect(reply, SIGNAL(finished()), this, SLOT(updateDownloadFinished()));

            timeOut->start(25000);
            connect(timeOut, SIGNAL(timeout()), this, SLOT(updateTimeOut()));
        }
    }
}

void PIG::updateTimeOut()
{
    updateAborted = true;
    finder();
}

void PIG::versionsReadyRead()
{
    remoteVersions = reply->readAll();
}

void PIG::versionsFinished()
{
    if (updateAborted) {
        timeOut->stop();
        reply->deleteLater();
        updateAborted = false;
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
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

        QTimer::singleShot(2000, this, SLOT(updateDownloadManager()));
    } else {
        timeOut->stop();
        reply->close();
        reply = 0;
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);

        QTimer::singleShot(2000, this, SLOT(finder()));
    }
}

void PIG::updateDownloadManager()
{
    if (remoteDbAvailable) {
        updateDownload = true;
        reply->close();
        reply = 0;
        fileName = "db.sqlite";
        url = remoteVersionsList[1];
        if (mRoot) mData = "update available"; mRoot->setProperty("status", mData);
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
        if (mRoot) mActive = true; mRoot->setProperty("showDecisionButton", mActive);
        if (remoteBinAvailable) {
            if (mRoot) mData = "there is a new version of the database and the binary package"; mRoot->setProperty("statusInformation", mData);
        } else {
            if (mRoot) mData = "there is a new version of the database"; mRoot->setProperty("statusInformation", mData);
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
        if (mRoot) mData = "update available"; mRoot->setProperty("status", mData);
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
        if (mRoot) mActive = true;  mRoot->setProperty("showDecisionButton", mActive);
        if (mRoot) mData = "there is a new version of the binary package"; mRoot->setProperty("statusInformation", mData);
    } else {
      reply->close();
      reply = 0;
      if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);

      finder();
    }
}

void PIG::updateDownloadReadyRead()
{
    file->write(reply->readAll());
}

void PIG::updateDownloadFinished()
{
    static short int pkgToDownloadControl = 0;
    QString hashDb = remoteVersionsList[2];
    #ifdef _WIN32
        QString fileOriginDb = "C:/tmp/db.sqlite";
        QString fileTarget = rootPath+".pig/db.sqlite";
        QString md5SumPathDb = fileOriginDb;
        QString fileOriginBin = "C:/tmp/PIG.exe";
        QString hashBin = remoteVersionsList[8];
        QString md5SumPathBin = "C:/tmp/PIG.exe";
    #else
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
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
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
            if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
            if (!remoteBinAvailable) {
                QFileInfo file(dbPath);
                if (file.isFile())
                    removeFile(dbPath);
                if (hashCalculation(md5SumPathDb, hashDb)) {
                        if (!copyFile(fileOriginDb, fileTarget)) {
                            #ifdef _WIN32
                                if (mRoot) mData = "PERMISSION DENIED"; mRoot->setProperty("status", mData);
                                if (mRoot) mData = "restart the application with administrator rights"; mRoot->setProperty("statusInformation", mData);
                            #else
                                if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
                                if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
                                QTimer::singleShot(3000, this, SLOT(finder()));
                            #endif
                        } else {
                            removeFile(fileOriginDb);
                            if (mRoot) mData = "updated database"; mRoot->setProperty("status", mData);
                            QTimer::singleShot(3000, this, SLOT(finder()));
                        }
                } else {
                   removeFile(fileOriginDb);
                   finder();
                }
            } else {
                if (hashCalculation(md5SumPathBin, hashBin)) {
                    #ifdef _WIN32
                        if (mRoot) mData = "updated"; mRoot->setProperty("status", mData);
                        if (mRoot) mData = "restart the application to make the changes effective"; mRoot->setProperty("statusInformation", mData);
                        if (mRoot) mActive = true; mRoot->setProperty("restart", mActive);
                    #else
                        if (mRoot) mData = "updated"; mRoot->setProperty("status", mData);
                        if (mRoot) mData = "authorization is required"; mRoot->setProperty("statusInformation", mData);
                        if (mRoot) mActive = true; mRoot->setProperty("authorize", mActive);
                    #endif
                } else {
                    removeFile(fileOriginBin);
                    finder();
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

                    updateHttp();
                } else {
                    removeFile(fileOriginDb);
                    if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
                    finder();
                }
            } else {
                reply->close();
                reply = 0;
                updateCleanAll = true;
                if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
                if (hashCalculation(md5SumPathBin, hashBin)) {
                    QFileInfo file(dbPath);
                    if (file.isFile())
                        removeFile(dbPath);
                    if (!copyFile(fileOriginDb, fileTarget)) {
                        #ifdef _WIN32
                            if (mRoot) mData = "PERMISSION DENIED"; mRoot->setProperty("status", mData);
                            if (mRoot) mData = "restart the application with administrator rights"; mRoot->setProperty("statusInformation", mData);
                        #else
                            if (mRoot) mData = "UPDATE FAILED"; mRoot->setProperty("status", mData);
                            if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
                            QTimer::singleShot(3000, this, SLOT(finder()));
                        #endif
                    } else {
                        removeFile(fileOriginDb);
                        #ifdef _WIN32
                            if (mRoot) mData = "updated"; mRoot->setProperty("status", mData);
                            if (mRoot) mData = "restart the application to make the changes effective"; mRoot->setProperty("statusInformation", mData);
                            if (mRoot) mActive = true; mRoot->setProperty("restart", mActive);
                        #else
                            if (mRoot) mData = "updated"; mRoot->setProperty("status", mData);
                            if (mRoot) mData = "authorization is required"; mRoot->setProperty("statusInformation", mData);
                            if (mRoot) mActive = true; mRoot->setProperty("authorize", mActive);
                        #endif
                    }
                } else {
                    removeFile(fileOriginDb);
                    removeFile(fileOriginBin);
                    finder();
                }
            }
        }

    }else {
        timeOut->stop();
        if (file)
            file->close();
        reply->close();
        reply = 0;
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);

        finder();
    }
}

void PIG::updateRestartApp()
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
            if(mRoot) mData = "PERMISSION DENIED"; mRoot->setProperty("status", mData);
            if(mRoot) mData = "restart the application with administrator rights"; mRoot->setProperty("statusInformation", mData);
        } else {
            updateControl();
        }
    #else
        QString userName = getenv("USER");
        QString updatePathArgs = "gksu PXupdate "+userName+" &";
        execUpdate = new QProcess(this);
        connect(execUpdate, SIGNAL(started()), this, SLOT(updateControl()));
        execUpdate->start("/bin/bash", QStringList() << "-c" << updatePathArgs);
    #endif
}

void PIG::updateControl()
{
    #ifdef _WIN32
        if (remoteBinAvailable && !remoteDbAvailable) {
            if (db.open()) {
                QSqlQuery qry;
                qry.prepare("UPDATE PxData SET BinVersion='"+remoteVersionsList[6]+"'");
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
                    qry.prepare("UPDATE PxData SET BinVersion='"+remoteVersionsList[6]+"'");
                    qry.exec();
                    db.close();
                }
            }
            exit(0);
        } else {
            if (mRoot) mData = "UPDATE FAILED"; mRoot->setProperty("status", mData);
            if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);

            QTimer::singleShot(3000, this, SLOT(finder()));
        }
    #endif
}

// Finder
void PIG::finder()
{
    delete networkAccess; 
    delete networkAccessConfig;
    
    QFileInfo file(dbPath);
    if (!file.isFile()) {
        errorDbHelper();
    } else {
        if (db.open()) {
            QSqlQuery qry;
            qry.prepare("SELECT DbVersion, BinVersion, Release, Category, NCategory, Pornstar, NPornstar FROM PxData, FiltersData");
            if (!qry.exec()) {
                db.close();
                errorDbHelper();
            } else {
                qry.next();
                localDbVersion = qry.value(0).toInt();
                localBinVersion = qry.value(1).toInt();
                localRelease = qry.value(2).toInt();
                QStringList categoryList = qry.value(3).toString().split(",");
                QStringList nCategoryList = qry.value(4).toString().split(",");
                QStringList pornstarList = qry.value(5).toString().split(",");
                QStringList nPornstarList = qry.value(6).toString().split(",");
                db.close();

                QString strLocalDbVersion = QString::number(localDbVersion);
                QString strLocalBinVersion = QString::number(localBinVersion);
                QString strLocalRelease = QString::number(localRelease);

                categoryList.prepend(QString::number(categoryList.count()));
                pornstarList.prepend(QString::number(pornstarList.count()));

                if (mRoot) mData = strLocalDbVersion; mRoot->setProperty("dataBaseVersion", mData);
                if (mRoot) mData = strLocalBinVersion; mRoot->setProperty("binaryVersion", mData);
                if (mRoot) mData = strLocalRelease; mRoot->setProperty("release", mData);
                if (mRoot) mDataList = categoryList; mRoot->setProperty("categoryList", mDataList);
                if (mRoot) mDataList = nCategoryList; mRoot->setProperty("nCategoryList", mDataList);
                if (mRoot) mDataList = pornstarList; mRoot->setProperty("pornstarList", mDataList);
                if (mRoot) mDataList = nPornstarList; mRoot->setProperty("nPornstarList", mDataList);
            }
        } else {
            errorDbHelper();
        }
    }
    if (mRoot) mActive = true; mRoot->setProperty("showFinder", mActive);
}

void PIG::finderDb(const QString inputText, QString category, QString pornstar, int offset, bool init)
{
    if (!db.open()) {
        if (mRoot) mActive = true;  mRoot->setProperty("showDbError", mActive);
        if (mRoot) mData = "ERROR IN DATABASE"; mRoot->setProperty("status", mData);
        if (mRoot) mData = "database does not exist or is corrupted"; mRoot->setProperty("statusInformation", mData);
    } else {
        row = 0;
       _list.clear();
        QString strOffset = QString::number(offset);
        QSqlQuery qry;
            qry.prepare( "SELECT Title, Pornstar, Quality, Collaborator, Category, UrlCover, UrlPoster, UrlPreview, UrlVideos FROM Films WHERE Title LIKE '%"+inputText+"%' AND Category LIKE '%"+category+"%' AND Pornstar LIKE '%"+pornstar+"%' ORDER BY Title ASC LIMIT 1000 OFFSET '"+strOffset+"'" );
        if (!qry.exec()) {
            db.close();
            if (mRoot) mActive = true; mRoot->setProperty("showDbError", mActive);
            if (mRoot) mData = "ERROR IN DATABASE"; mRoot->setProperty("status", mData);
            if (mRoot) mData = "database does not exist or is corrupted"; mRoot->setProperty("statusInformation", mData);
        } else {
            if (init) {
                qry.last(); if(mRoot) mNumber = qry.at() + 1; mRoot->setProperty("totalFilms", mNumber); qry.first(); qry.previous();
            }

            for (row=0; qry.next()&&row<5; row++) {
                QString _title = qry.value(0).toString();
                QString _pornstars = qry.value(1).toString();
                QString _quality = qry.value(2).toString();
                QString _collaborator = qry.value(3).toString();
                QString _category = qry.value(4).toString();
                QString _urlCover = qry.value(5).toString();
                QString _urlPoster = qry.value(6).toString();
                QString _urlPreview = qry.value(7).toString();
                QString _urlVideos = qry.value(8).toString();
                _list << _title << _pornstars << _quality << _collaborator << _category << _urlCover << _urlPoster << _urlPreview << _urlVideos << inputText << category << pornstar;
            }
            db.close();

            if (!qry.last() && init) {
                if (mRoot) mActive = true; mRoot->setProperty("noResult", mActive);
            } else {
                if (init) {
                    if (mRoot) mNumber = row; mRoot->setProperty("n", mNumber);
                    if (mRoot) mDataList = _list; mRoot->setProperty("list", mDataList);
                    if (mRoot) mActive = true; mRoot->setProperty("showOutput", mActive);
                } else {
                    if (mRoot) mNumber = row; mRoot->setProperty("n", mNumber);
                    if (mRoot) mDataList = _list; mRoot->setProperty("list", mDataList);
                    if (mRoot) mActive = true; mRoot->setProperty("listUpdated", mActive);
                }
            }
        }
    }
}


// Player
void PIG::openPlayer(QString videoID)
{
    mPlayer = new VideoPlayer();
    mPlayer->open(videoID);

    container->hide();
    layout->addWidget(mPlayer);

    Esc->setEnabled(true);
}

void PIG::closePlayer()
{
    container->show();
    mPlayer->close();

    Esc->setEnabled(false);
    if(mRoot) mActive = true; mRoot->setProperty("playerClosed", mActive);
}

// FixDB
void PIG::errorDbHelper()
{
    if (mRoot) mData = "ERROR IN DATABASE"; mRoot->setProperty("status", mData);
    if (mRoot) mData = "database does not exist or is corrupted"; mRoot->setProperty("statusInformation", mData);
    if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
    if (mRoot) mActive = true; mRoot->setProperty("showFixDbButton", mActive);
}

void PIG::fixerDb()
{
    if (mRoot) mData = "updating"; mRoot->setProperty("status", mData);
    if (mRoot) mActive = true; mRoot->setProperty("showSpinner", mActive);
    url = "http://gamenetworkmanager.herokuapp.com/update";

    reply = networkAccess->get(QNetworkRequest(url));
    connect(reply, SIGNAL(readyRead()), this, SLOT(getUrlReadyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(getUrlFinished()));

    timeOut = new QTimer();
    timeOut->setSingleShot(true);
    connect(timeOut, SIGNAL(timeout()), this, SLOT(fixDbTimeOut()));
    timeOut->start(15000);
}

void PIG::fixerDbTimeOut()
{
    fixDbAborted = true;
    if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
    if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
}

void PIG::getUrlReadyRead()
{
    remoteUrl = reply->readAll();
}

void PIG::getUrlFinished()
{
    if (fixDbAborted) {
        timeOut->stop();
        reply->deleteLater();
        fixDbAborted = false;
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        timeOut->stop();
        reply->close();
        reply = 0;
        remoteUrlList = remoteUrl.split(",");
        url = remoteUrlList[1];
        fileName = "db.sqlite";
        #ifdef _WIN32
            file = new QFile("C:/tmp/"+fileName);
        #else
            file = new QFile("/tmp/"+fileName);
        #endif
        if (!file->open(QIODevice::WriteOnly)) {
            #ifdef _WIN32
                if (mRoot) mData = "PERMISSION DENIED"; mRoot->setProperty("status", mData);
                if (mRoot) mData = "restart the application with administrator rights"; mRoot->setProperty("statusInformation", mData);
            #else
                if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
                if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
            #endif
            if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
        } else {
            reply = networkAccess->get(QNetworkRequest(url));
            connect(reply, SIGNAL(readyRead()),this, SLOT(fixerReadyRead()));
            connect(reply, SIGNAL(finished()), this, SLOT(fixerFinished()));

            timeOut->start(25000);
            connect(timeOut, SIGNAL(timeout()), this, SLOT(fixDbTimeOut()));
        }
    } else {
        timeOut->stop();
        reply->close();
        reply = 0;
        if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
        if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);
        if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
    }
}

void PIG::fixerReadyRead()
{
    file->write(reply->readAll());
}

void PIG::fixerFinished()
{
    QString hashDb = remoteUrlList[2];
    #ifdef _WIN32
        QString fileOriginDb = "C:/tmp/db.sqlite";
        QString fileTarget = rootPath+".pig/db.sqlite";
        QString md5SumPathDb = fileOriginDb;
    #else
        QString fileOriginDb = "/tmp/db.sqlite";
        QString fileTarget = QDir::homePath()+"/.pig/db.sqlite";
        QString md5SumPathDb = fileOriginDb;
    #endif
    if (mRoot) mActive = false; mRoot->setProperty("showSpinner", mActive);

    if (fixDbAborted) {
        timeOut->stop();
        fixDbAborted = false;
        if (file)
            file->close();
        reply->deleteLater();
        return;
    }
    //POR ACA EL ERROR
    if (reply->error() == QNetworkReply::NoError) {
        timeOut->stop();
        file->flush();
        file->close();
        reply->close();
        reply = 0;

        if (hashCalculation(md5SumPathDb, hashDb)) {
            QFileInfo file(dbPath);
            if (file.isFile())
                removeFile(dbPath);
            if (!copyFile(fileOriginDb, fileTarget)) {
                #ifdef _WIN32
                    if (mRoot) mData = "PERMISSION DENIED"; mRoot->setProperty("status", mData);
                    if (mRoot) mData = "restart the application with administrator rights"; mRoot->setProperty("statusInformation", mData);
                #else
                    if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
                    if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
                #endif
            } else {
                removeFile(fileOriginDb);
                if (mRoot) mData = "updated database"; mRoot->setProperty("status", mData);
                if (mRoot) mData = "restart the application to make the changes effective"; mRoot->setProperty("statusInformation", mData);
            }
        } else {
            removeFile(fileOriginDb);
            if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
            if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
        }

    } else {
        if (mRoot) mData = "FAILED TO UPDATE THE DATABASE"; mRoot->setProperty("status", mData);
        if (mRoot) mData = "try later"; mRoot->setProperty("statusInformation", mData);
    }
}

// Helpers
bool PIG::hashCalculation(QString md5SumPath, QString md5SumHash)
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

bool PIG::copyFile(const QString& fileOrigin, const QString& fileTarget)
{
    bool result = QFile::copy(fileOrigin, fileTarget);
    return result;
}
bool PIG::removeFile(const QString &fileOrigin)
{
    bool result = QFile::remove(fileOrigin);
    return result;
}

void PIG::close()
{
    delete qApp;
}

void PIG::setRootObject(QObject *root)
{
    if(mRoot!=0) mRoot->disconnect(this); mRoot = root;

    if(mRoot) connect(mRoot, SIGNAL(updateAccept()), this, SLOT(updateHttp()));
    if(mRoot) connect(mRoot, SIGNAL(updateCancel()), this, SLOT(finder()));
    if(mRoot) connect(mRoot, SIGNAL(updateRestart()), this, SLOT(updateRestartApp()));
    if(mRoot) connect(mRoot, SIGNAL(findDb(QString, QString, QString, int, bool)),
                             this, SLOT(finderDb(QString, QString, QString, int, bool)));
    if(mRoot) connect(mRoot, SIGNAL(openPlayer(QString)), this, SLOT(openPlayer(QString)));
    if(mRoot) connect(mRoot, SIGNAL(passManager(QString, bool, bool)), this, SLOT(passManager(QString, bool, bool)));
    if(mRoot) connect(mRoot, SIGNAL(fixDb()), this, SLOT(fixerDb()));
    if(mRoot) connect(mRoot, SIGNAL(quit()), this, SLOT(close()));

    passManager("", true, false);
}
