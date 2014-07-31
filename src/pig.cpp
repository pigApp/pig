#include "pig.h"

#include <stdlib.h>

#include <QLayoutItem>
#include <QDir>
#include <QFile>
#include <QTextStream>

PIG::PIG(QWidget *parent) : QWidget(parent), mRoot(0)
{
    mPassword = NULL;
    mUpdate = NULL;
    mTorrent = NULL;

    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
}

PIG::~PIG()
{
    if (mPassword != NULL)
        delete mPassword;
    if (mUpdate != NULL)
        delete mUpdate;
    if (mTorrent != NULL)
        delete mTorrent;
    //cleanUp();
    mRoot->disconnect(this);
    exit(0);
}

void PIG::set_root_object(QObject *root)
{
    if(mRoot!=0) mRoot->disconnect(this); mRoot = root;

    if(mRoot) connect(mRoot, SIGNAL(passwordHandleSIGNAL_QML(QString, bool, bool)), this, SLOT(password_handle(QString, bool, bool)));
    if(mRoot) connect(mRoot, SIGNAL(findSIGNAL_QML(QString, QString, QString, QString, QString, int, bool)), this, SLOT(find(QString, QString, QString, QString, QString, int, bool)));
    if(mRoot) connect(mRoot, SIGNAL(torrentHandleSIGNAL_QML(QString, QString, bool)), this, SLOT(torrent_handle(QString, QString, bool)));
    if(mRoot) connect(mRoot, SIGNAL(quitSIGNAL_QML()), this, SLOT(quit()));

#ifdef _WIN32
    QString target = "C:/pig/.pig/db.sqlite";
    QString tmp = "C:/tmp/pig/";
#else
    QString target = QDir::homePath()+"/.pig/db.sqlite";
    QString tmp = QDir::homePath()+"/.pig/tmp/";
#endif
    QFile file(target);
    if (file.exists()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(target);
        QDir dir(tmp);
        if (!dir.exists())
            dir.mkdir(tmp);

        mPassword = new Password();
        password_handle("", true, false);
    } else {
        QTimer::singleShot(10, this, SLOT(error_database()));
    }
}

// Password
void PIG::password_handle(QString plain, bool init, bool write)
{
    if (!init && !write) {
        if (mPassword->right(plain)) {
            mRoot->setProperty("require_password", false);
            update_handle();
        } else {
            mRoot->setProperty("fail_password", true);
        }
    } else if (init) {
        if (mPassword->require()) {
            mRoot->setProperty("require_password", true);
            init = false;
        } else {
            QTimer::singleShot(10, this, SLOT(update_handle()));
            init = false;
        }
    } else if (write) {
        if (mPassword->write(plain)) {
            mRoot->setProperty("ok_password", true);
        } else {
            mRoot->setProperty("fail_password", true);
        }
    }
}

// Update
void PIG::update_handle()
{
    emit showUpdateSIGNAL();

    mUpdate = new Update();
    mUpdate->db = db;
    mUpdate->_root = mRoot;
    mUpdate->check();

    connect(mUpdate, SIGNAL(forwardSIGNAL()), this, SLOT(start()));
    connect(mUpdate, SIGNAL(errorDatabaseSIGNAL()), this, SLOT(error_database()));
    connect(mRoot, SIGNAL(skipSIGNAL_QML()), this, SLOT(start()));
    connect(mRoot, SIGNAL(getFilesSIGNAL_QML()), mUpdate, SLOT(get_files()));
}

// Start
void PIG::start()
{
    delete mUpdate;
    mUpdate = NULL;

    if (db.open()) {
        QSqlQuery qry;
        qry.prepare("SELECT DatabaseVersion, BinaryVersion, Release, Category, NCategory, Pornstar, NPornstar FROM PigData, FiltersData");
        if (!qry.exec()) {
            db.close();
            error_database();
        } else {
            qry.next();
            int currentDatabaseVersion = qry.value(0).toInt();
            int currentBinaryVersion = qry.value(1).toInt();
            int currentRelease = qry.value(2).toInt();
            QStringList categoryList = qry.value(3).toString().split(",");
            QStringList nCategoryList = qry.value(4).toString().split(",");
            QStringList pornstarList = qry.value(5).toString().split(",");
            QStringList nPornstarList = qry.value(6).toString().split(",");
            db.close();

            QString strCurrentDatabaseVersion = QString::number(currentDatabaseVersion);
            QString strCurrentBinaryVersion = QString::number(currentBinaryVersion);
            QString strCurrentRelease= QString::number(currentRelease);

            categoryList.prepend(QString::number(categoryList.count()));
            pornstarList.prepend(QString::number(pornstarList.count()));

            mRoot->setProperty("databaseVersion", strCurrentDatabaseVersion);
            mRoot->setProperty("binaryVersion", strCurrentBinaryVersion);
            mRoot->setProperty("release", strCurrentRelease);
            mRoot->setProperty("categoryList", categoryList);
            mRoot->setProperty("nCategoryList", nCategoryList);
            mRoot->setProperty("pornstarList", pornstarList);
            mRoot->setProperty("nPornstarList", nPornstarList);

            mTorrent = new Torrent();
            mTorrent->_pig = this;
            mTorrent->_root = mRoot;
        }
    } else {
        error_database();
    }

#ifdef _WIN32
    QString target = "C:/pig/.pig/news.txt";
#else
    QString target = QDir::homePath()+"/.pig/news";
#endif
    QFile file(target);
    if (file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        bool head = true;
        QString bn;
        QString dbn;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.isEmpty()) head = false;
            if (head) {
                bn.append(line+"\n");
            } else {
                if(!line.isEmpty())
                    dbn.append(line+"\n");
            }
        }
        file.close();
        file.remove();
        mRoot->setProperty("binaryNews", bn);
        mRoot->setProperty("databaseNews", dbn);
        mRoot->setProperty("news", true);
    }

    emit startSIGNAL();
}

//Find
void PIG::find(const QString input, QString pornstar, QString category, QString quality, QString full, int offset, bool init)
{
    static QStringList _list;
    int row = 0;

    if (!db.open()) {
        error_database();
    } else {
        _list.clear();
        QString strOffset = QString::number(offset);
        QSqlQuery qry;
            qry.prepare("SELECT Title, Cas, Category, Quality, Full, UrlPoster, UrlCover, UrlScreens, Torrent FROM Films WHERE Title LIKE '%"+input+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE '%"+category+"%' AND Quality LIKE '%"+quality+"%' AND Full LIKE '%"+full+"%' ORDER BY Title ASC LIMIT 1000 OFFSET '"+strOffset+"'");
        if (!qry.exec()) {
            db.close();
            error_database();
        } else {
            if (init) {
                qry.last();
                mRoot->setProperty("totalFilms", qry.at()+1);
                qry.first();
                qry.previous();
            }
            for (row=0; qry.next()&&row<5; row++) {
                QString _title = qry.value(0).toString();
                QString _cast = qry.value(1).toString();
                QString _category = qry.value(2).toString();
                QString _quality = qry.value(3).toString();
                QString _full = qry.value(4).toString();
                QString _urlPoster = qry.value(5).toString();
                QString _urlCover = qry.value(6).toString();
                QString _urlScreens = qry.value(7).toString();
                QString _torrent = qry.value(8).toString();
                _list << _title << _cast << _category << _quality << _full << _urlPoster << _urlCover << _urlScreens << _torrent;
            }
            db.close();

            if (!qry.last() && init) {
                emit noResultSIGNAL();
            } else {
                if (init) {
                    mRoot->setProperty("n", row);
                    mRoot->setProperty("list", _list);
                    emit showOutputSIGNAL();
                } else {
                    mRoot->setProperty("n", row);
                    mRoot->setProperty("list", _list);
                    emit listUpdatedSIGNAL();
                }
            }
        }
    }
}

// Torrent
void PIG::torrent_handle(QString magnet, QString scenne, bool abort)
{
    if (!abort) {
        mTorrent->scenne = scenne.toInt();
        mTorrent->doConnect(magnet);
    } else {
        mTorrent->stop();
    }
}

// Player
void PIG::player_handle(const QString absoluteFilePath, bool init, bool sandbox, bool fileReady, bool abort)
{
    if (!abort) {
        if (init) {
            mPlayer = new VideoPlayer();
            mPlayer->_torrent = mTorrent;
            mPlayer->_pig = this;
            mTorrent->_player = mPlayer;
        }
        if (sandbox) {
            mPlayer->sandbox(absoluteFilePath);
            emit checkingFileSIGNAL();
        }
        if (fileReady) {
            mTorrent->toPlayer = true;
            mPlayer->showFullScreen();
            this->hide();
            emit fileReadySIGNAL();
        }
    } else {
        this->show();
        emit abortTorrentSIGNAL();
        mPlayer->close();
        delete mPlayer;
    }
}

void PIG::cleanUp()
{
    #ifdef _WIN32
        QString target = "C:/tmp/pig/";
    #else
        QString target = QDir::homePath()+"/.pig/tmp/";
    #endif
        QDir tmp(target);

        tmp.setFilter(QDir::NoDotAndDotDot | QDir::Files);
        foreach(QString tmpItem, tmp.entryList())
            tmp.remove(tmpItem);

        tmp.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
        foreach(QString tmpItem, tmp.entryList()) {
            QDir subDir(tmp.absoluteFilePath(tmpItem));
            subDir.removeRecursively();
        }
}

// ErrorDb
void PIG::error_database()
{
    emit showErrorDatabaseSIGNAL();
}

void PIG::quit()
{
    this->~PIG();
}
