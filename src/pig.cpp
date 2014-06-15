#ifdef _WIN32
#include <windows.h>
#endif

#include <QDir>
#include <QUuid>

#include "tcpSocket.h"
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
    if (init) { // Comprueba si se usa password.
        if (mPassword->requirePassword()) {
            mRoot->setProperty("requirePass", true); // Si se usa password, activa el input para escribirlo y desactiva setear password.
            init = false;
        } else {
            QStringList args = qApp->arguments(); // Si no se usa password, inicia.
            if (args.last() == "WITHOUT_UPDATE")
                finder();
            else
                QTimer::singleShot(1000, this, SLOT(updateHandle()));
            init = false;
        }
    } else if (!init && !write) { // Llama a rightPassword y le pasa el password ingresado para comprobarlo.
        if (mPassword->rightPassword(plain)) { // Si el password coincide, inicia.
            QStringList args = qApp->arguments();
            if (args.last() == "WITHOUT_UPDATE") {
                finder();
            } else {
                mRoot->setProperty("showAskPass", false);
                QTimer::singleShot(350, this, SLOT(updateHandle()));
            }
        } else {
            mRoot->setProperty("failPass", true); // Si el password no coincide envia un bool true para que se muestre el mensaje, la contraseña no coincide.
        }
    } else if (write) { // Para setear el password con la aplicacion ya iniciada.
        if (mPassword->writePassword(plain)) {
            mRoot->setProperty("okPass", true); // Se escribio.
        } else {
            mRoot->setProperty("failPass", true); // No se escribio.
        }
    }
}

// Update
void PIG::updateHandle()
{
    mUpdate = new Update();
    mUpdate->db = db;
    mUpdate->init(mRoot);//mUpdate->doCheck(mRoot);
    emit showUpdateSIGNAL();

    connect(mUpdate, SIGNAL(updateCallFinder()), this, SLOT(finder()));
    connect(mUpdate, SIGNAL(updateErrorDb()), this, SLOT(errorDb()));
}

// Finder
void PIG::finder()
{
    delete mUpdate;

    if (db.open()) {
        QSqlQuery qry;
        qry.prepare("SELECT DbVersion, BinVersion, Release, Category, NCategory, Pornstar, NPornstar FROM PigData, FiltersData");
        if (!qry.exec()) {
            db.close();
            errorDb();
        } else {
            qry.next();
            int currentDBVersion = qry.value(0).toInt();
            int currentBinaryVersion = qry.value(1).toInt();
            int currentBinaryRelease = qry.value(2).toInt();
            QStringList categoryList = qry.value(3).toString().split(",");
            QStringList nCategoryList = qry.value(4).toString().split(",");
            QStringList pornstarList = qry.value(5).toString().split(",");
            QStringList nPornstarList = qry.value(6).toString().split(",");
            db.close();

            QString strCurrentDBVersion = QString::number(currentDBVersion);
            QString strCurrentBinaryVersion = QString::number(currentBinaryVersion);
            QString strCurrentBinaryRelease= QString::number(currentBinaryRelease);

            categoryList.prepend(QString::number(categoryList.count()));
            pornstarList.prepend(QString::number(pornstarList.count()));

            mRoot->setProperty("dataBaseVersion", strCurrentDBVersion);
            mRoot->setProperty("binaryVersion", strCurrentBinaryVersion);
            mRoot->setProperty("binaryRelease", strCurrentBinaryRelease);
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
void PIG::getTorrent(QString host, QString url, QString scenneID)
{
    scenne = scenneID.toInt();

    int base = 1000000;
    int randomTorrentID = qrand() % base;
    QString file = QString::number(randomTorrentID);

    //TcpSocket s; // TODO: Instanciar el socket desde aca no de .h.
    mSocket.host = host;
    mSocket.url = url;
    mSocket.file = file+".torrent";
    mSocket.order = "getTorrent";
    mSocket.doConnect();
    //mSocket.close();
    connect(&mSocket, SIGNAL(fileReady(QString, QString)), this, SLOT(torrentHandle(QString, QString)));
}

void PIG::torrentHandle(QString path, QString file)
{
    mTorrent = new Torrent(this, this);
    mTorrent->download(path, file, scenne);
}

// Player
void PIG::playerHandle(const QString path, const QString file)
{
    QRect screen = window->geometry();

    mPlayer = new VideoPlayer(window, screen.width(), screen.height());
    mPlayer->open(path+file, mTorrent);

    container->hide();
    layout->addWidget(mPlayer);

    SpaceBar = new QShortcut(window);
    SpaceBar->setEnabled(true);
    SpaceBar->setKey(Qt::Key_Space);
    connect(SpaceBar, SIGNAL(activated()), mPlayer, SLOT(playPause()));

    UpArrow = new QShortcut(window);
    UpArrow->setEnabled(true);
    UpArrow->setKey(Qt::Key_Up);
    connect(UpArrow, SIGNAL(activated()), mPlayer, SLOT(setPositiveVolume()));

    DownArrow = new QShortcut(window);
    DownArrow->setEnabled(true);
    DownArrow->setKey(Qt::Key_Down);
    connect(DownArrow, SIGNAL(activated()), mPlayer, SLOT(setNegativeVolume()));
}

void PIG::closePlayer()
{
    container->show();
    mPlayer->close();
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

    //if(mRoot) connect(mRoot, SIGNAL(updateAccept()), this, SLOT(updateHttp())); // TODO: Reemplazar this por mUpdate.
    //if(mRoot) connect(mRoot, SIGNAL(updateCancel()), this, SLOT(finder()));
    //if(mRoot) connect(mRoot, SIGNAL(updateRestart()), this, SLOT(updateRestartApp()));

    if(mRoot) connect(mRoot, SIGNAL(findDb(QString, QString, QString, int, bool)), this, SLOT(findDb(QString, QString, QString, int, bool)));
    if(mRoot) connect(mRoot, SIGNAL(getTorrent(QString, QString, QString)), this, SLOT(getTorrent(QString, QString, QString)));
    if(mRoot) connect(mRoot, SIGNAL(passwordHandle(QString, bool, bool)), this, SLOT(passwordHandle(QString, bool, bool)));
    if(mRoot) connect(mRoot, SIGNAL(quit()), this, SLOT(quit()));

    passwordHandle("", true, false);
}

void PIG::quit()
{
    qApp->exit();
    exit(0);
}
