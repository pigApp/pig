#include "pig.h"
#include "tcpSocket.h"

#include <stdlib.h>
#include <QDir>
#include <QFile>
#include <QTextStream>

PIG::PIG(QWidget *parent) : QWidget(parent), mRoot(0)
{
    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    setMouseTracking(true);
}

PIG::~PIG()
{
}

void PIG::setRootObject(QObject *root)
{
    if(mRoot!=0) mRoot->disconnect(this); mRoot = root;

    if(mRoot) connect(mRoot, SIGNAL(passwordHandle(QString, bool, bool)), this, SLOT(passwordHandle(QString, bool, bool)));
    if(mRoot) connect(mRoot, SIGNAL(find(QString, QString, QString, int, bool)), this, SLOT(find(QString, QString, QString, int, bool)));
    if(mRoot) connect(mRoot, SIGNAL(torrentHandle(QString, QString)), this, SLOT(torrentHandle(QString, QString)));
    if(mRoot) connect(mRoot, SIGNAL(quit()), this, SLOT(quit()));

#ifdef _WIN32
    QString target = "C:/pig/.pig/db.sqlite";
    QString tmp = "C:/tmp/pig/";
#else
    QString target = QDir::homePath()+"/.pig/db.sqlite";
    QString tmp = "/tmp/pig/";
#endif
    QFile file(target);
    if (file.exists()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(target);
        QDir dir(tmp);
        if (!dir.exists())
            dir.mkdir(tmp);

        passwordHandle("", true, false);
    } else {
        QTimer::singleShot(10, this, SLOT(errorDb()));
    }
}

// Password
void PIG::passwordHandle(QString plain, bool init, bool write)
{
    if (!init && !write) {
        if (mPassword->right(plain)) {
            mRoot->setProperty("require_password", false);
            updateHandle();
        } else {
            mRoot->setProperty("fail_password", true);
        }
    } else if (init) {
        if (mPassword->require()) {
            mRoot->setProperty("require_password", true);
            init = false;
        } else {
            QTimer::singleShot(10, this, SLOT(updateHandle()));
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
void PIG::updateHandle()
{
    emit showUpdateSIGNAL();

    mUpdate = new Update();
    mUpdate->db = db;
    mUpdate->_root = mRoot;
    mUpdate->doCheck();

    connect(mUpdate, SIGNAL(forward()), this, SLOT(start()));
    connect(mUpdate, SIGNAL(errorDb()), this, SLOT(errorDb()));
    connect(mRoot, SIGNAL(skip()), this, SLOT(start()));
    connect(mRoot, SIGNAL(getFiles()), mUpdate, SLOT(getFiles()));
}

// Start
void PIG::start()
{
    mUpdate = 0;
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

void PIG::find(const QString inputText, QString category, QString pornstar, int offset, bool init)
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
        if (!qry.exec()) { // TODO: Cambiar Ponstar por Cast. Tambien en la db.
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
    mPlayer = new VideoPlayer(this, this->geometry().width(), this->geometry().height());
    mPlayer->_torrent = mTorrent;
    mTorrent->_player = mPlayer;
    mPlayer->doRun(absoluteFilePath);

    container->hide();
    layout->addLayout(mPlayer->layout);
}

void PIG::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        mPlayer->playPause();
    } else if (event->key() == Qt::Key_Left) {
        mPlayer->skip_key_value = -10000;
        mPlayer->sliderReleased();
    } else if (event->key() == Qt::Key_Right) {
        mPlayer->skip_key_value = 10000;
        mPlayer->sliderReleased();
    } else if (event->key() == Qt::Key_Up) {
        mPlayer->setVolume(5);
    } else if (event->key() == Qt::Key_Down) {
        mPlayer->setVolume(-5);
    } else if (event->key() == Qt::Key_Escape) { // TODO: Comprobar que funcione.
        closePlayer();
    } else if (event->key() == (Qt::Key_Escape && Qt::ControlModifier)) { // TODO: Comprobar que funcione.
        quit();
    }
}

void PIG::mousePressEvent(QMouseEvent *event)
{
    qDebug() << event;
}

void PIG::closePlayer()
{
    container->show();
    //mPlayer->close();
    delete mPlayer;

    emit hidePlayerLayerSIGNAL();
}

// ErrorDb
void PIG::errorDb()
{
    emit showErrorDbSIGNAL();
}

void PIG::quit()
{
    this->destroy(); // TODO: Salir bien.
    //qApp->quit();
    //exit(0);
}
