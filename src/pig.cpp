#include "pig.h"
#include "tcpSocket.h"

#include <stdlib.h>
#include <QDir>


PIG::PIG(QObject *parent) : QObject(parent), mRoot(0)
{
    window = new QWidget;
    window->setMouseTracking(true);
    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    layout->setSpacing(0);
    window->setLayout(layout);

    Esc = new QShortcut(window);
    Esc->setKey(Qt::Key_Escape);
    Esc->setEnabled(false);
    connect(Esc, SIGNAL(activated()), this, SLOT(closePlayer()));

    Quit = new QShortcut(window);
    Quit->setKey(Qt::Key_Escape && Qt::ShiftModifier);
    connect(Quit, SIGNAL(activated()), this, SLOT(quit()));

    #ifdef _WIN32
        QString dbPath = "C:/pig/.pig/db.sqlite";
        QString tmpPath = "C:/tmp/pig/";
    #else
        QString dbPath = QDir::homePath()+"/.pig/db.sqlite";
        QString tmpPath = "/tmp/pig/";
    #endif

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    QDir tmpDir(tmpPath);
    if (!tmpDir.exists())
        tmpDir.mkdir(tmpPath);
}

PIG::~PIG()
{
}

// Password
void PIG::passwordHandle(QString plain, bool init, bool write)
{
    if (init) {
        if (mPassword->requirePassword()) {
            mRoot->setProperty("requirePass", true);
            init = false;
        } else {
            QStringList args = qApp->arguments();
            if (args.last() == "WITHOUT_UPDATE")
                finder();
            else
                updateHandle();
            init = false;
        }
    } else if (!init && !write) {
        if (mPassword->rightPassword(plain)) {
            QStringList args = qApp->arguments();
            if (args.last() == "WITHOUT_UPDATE") {
                finder();
            } else {
                mRoot->setProperty("showAskPass", false);
                updateHandle();
            }
        } else {
            mRoot->setProperty("failPass", true);
        }
    } else if (write) {
        if (mPassword->writePassword(plain)) {
            mRoot->setProperty("okPass", true);
        } else {
            mRoot->setProperty("failPass", true);
        }
    }
}

// Update
void PIG::updateHandle()
{
    mUpdate = new Update();
    mUpdate->db = db;
    mUpdate->_root = mRoot;
    mUpdate->doCheck();
    emit showUpdateSIGNAL();

    connect(mUpdate, SIGNAL(updateCallFinder()), this, SLOT(finder()));
    connect(mUpdate, SIGNAL(updateErrorDb()), this, SLOT(errorDb()));
    if(mRoot) connect(mRoot, SIGNAL(getFiles()), mUpdate, SLOT(getFiles()));
    if(mRoot) connect(mRoot, SIGNAL(restart()), mUpdate, SLOT(replaceBinaryAndRestart()));
}

// Finder
void PIG::finder()
{
    delete mUpdate;

    if (db.open()) {
        QSqlQuery qry;
        qry.prepare("SELECT DatabaseVersion, BinaryVersion, Release, Category, NCategory, Pornstar, NPornstar FROM PigData, FiltersData");
        if (!qry.exec()) {
            db.close();
            errorDb();
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
        }
    } else {
        errorDb();
    }
    emit showFinderSIGNAL();
}

void PIG::findDb(const QString inputText, QString category, QString pornstar, int offset, bool init)
{
    static QStringList _list;
    int row = 0;

    if (!db.open()) {
        errorDb();
    } else {
        _list.clear();
        QString strOffset = QString::number(offset);
        QSqlQuery qry;
            qry.prepare( "SELECT Title, Pornstar, Quality, Category, UrlCover, UrlPoster, UrlPreview, Torrent FROM Films WHERE Title LIKE '%"+inputText+"%' AND Category LIKE '%"+category+"%' AND Pornstar LIKE '%"+pornstar+"%' ORDER BY Title ASC LIMIT 1000 OFFSET '"+strOffset+"'" );
        if (!qry.exec()) {
            db.close();
            errorDb();
        } else {
            if (init) {
                qry.last();
                mRoot->setProperty("totalFilms", qry.at()+1);
                qry.first();
                qry.previous();
            }

            for (row=0; qry.next()&&row<5; row++) {
                QString _title = qry.value(0).toString();
                QString _pornstars = qry.value(1).toString();
                QString _quality = qry.value(2).toString();
                QString _category = qry.value(3).toString();
                QString _urlCover = qry.value(4).toString();
                QString _urlPoster = qry.value(5).toString();
                QString _urlPreview = qry.value(6).toString();
                QString _torrent = qry.value(7).toString();
                _list << _title << _pornstars << _quality << _category << _urlCover << _urlPoster << _urlPreview << _torrent << inputText << category << pornstar;
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
void PIG::torrentHandle(QString magnetUrl, QString scenne)
{
    mTorrent = new Torrent();
    mTorrent->_pig = this;
    mTorrent->_root = mRoot;
    mTorrent->scenne = scenne.toInt();
    mTorrent->doRun(magnetUrl);
}

// Player
void PIG::playerHandle(const QString absoluteFilePath)
{
    mPlayer = new VideoPlayer(this, window->geometry().width(), window->geometry().height());
    mPlayer->_torrent = mTorrent;
    mTorrent->_player = mPlayer; // TODO: Asegurarse en torrent.cpp que sea un puntero valido antes de llamar a progress().
    mPlayer->doRun(absoluteFilePath);
                                 // TODO: Desde aca, pasarlo a otro slot, que sea llamado si el video es valido.
    container->hide();
    layout->addLayout(mPlayer->layout);

    SpaceBar = new QShortcut(window);
    SpaceBar->setEnabled(true);
    SpaceBar->setKey(Qt::Key_Space);
    UpArrow = new QShortcut(window);
    UpArrow->setEnabled(true);
    UpArrow->setKey(Qt::Key_Up);
    DownArrow = new QShortcut(window);
    DownArrow->setEnabled(true);
    DownArrow->setKey(Qt::Key_Down);
    connect(SpaceBar, SIGNAL(activated()), mPlayer, SLOT(playPauseForUser()));
    connect(UpArrow, SIGNAL(activated()), mPlayer, SLOT(setPositiveVolume()));
    connect(DownArrow, SIGNAL(activated()), mPlayer, SLOT(setNegativeVolume()));
}

void PIG::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "=======================MOUSE_MOVE";
}

void PIG::closePlayer()
{
    container->show();
    //mPlayer->close();
    delete mPlayer;

    SpaceBar->setEnabled(false);
    UpArrow->setEnabled(false);
    DownArrow->setEnabled(false);
    Esc->setEnabled(false);

    emit hidePlayerLayerSIGNAL();
}

// ErrorDb
void PIG::errorDb()
{
    emit showErrorDbMsgSIGNAL();
}

void PIG::setRootObject(QObject *root)
{
    if(mRoot!=0) mRoot->disconnect(this); mRoot = root;

    if(mRoot) connect(mRoot, SIGNAL(passwordHandle(QString, bool, bool)), this, SLOT(passwordHandle(QString, bool, bool)));
    if(mRoot) connect(mRoot, SIGNAL(updateCancel()), this, SLOT(finder()));
    if(mRoot) connect(mRoot, SIGNAL(findDb(QString, QString, QString, int, bool)), this, SLOT(findDb(QString, QString, QString, int, bool)));
    if(mRoot) connect(mRoot, SIGNAL(torrentHandle(QString, QString)), this, SLOT(torrentHandle(QString, QString)));
    if(mRoot) connect(mRoot, SIGNAL(quit()), this, SLOT(quit()));

    passwordHandle("", true, false);
}

void PIG::quit()
{
    qApp->exit();
    exit(0);
}
