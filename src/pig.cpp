#ifdef _WIN32
#include <windows.h>
#endif

#include <QDir>

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
        dbPath  = "C:/pig/.pig/db.sqlite";
        tmpPath = "C:/tmp/pig/";
    #else
        dbPath  = QDir::homePath()+"/.pig/db.sqlite";
        tmpPath = "/tmp/pig/";
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
void PIG::passwordManager(QString plain, bool init, bool write)
{
    if (init) { // Comprueba si se usa password.
        if (mPass->requirePassword()) {
            mRoot->setProperty("requirePass", true); // Si se usa password, activa el input para escribirlo y desactiva setear password.
            init = false;
        } else {
            QStringList args = qApp->arguments(); // Si no se usa password, inicia.
            if (args.last() == "WITHOUT_UPDATE")
                finder();
            else
                QTimer::singleShot(1000, this, SLOT(update()));
            init = false;
        }
    } else if (!init && !write) { // Llama a rightPassword y le pasa el password ingresado para comprobarlo.
        if (mPass->rightPassword(plain)) { // Si el password coincide, inicia.
            QStringList args = qApp->arguments();
            if (args.last() == "WITHOUT_UPDATE") {
                finder();
            } else {
                mRoot->setProperty("showAskPass", false);
                QTimer::singleShot(350, this, SLOT(update()));
            }
        } else {
            mRoot->setProperty("failPass", true); // Si el password no coincide envia un bool true para que se muestre el mensaje, la contraseña no coincide.
        }
    } else if (write) { // Para setear el password con la aplicacion ya iniciada.
        if (mPass->writePassword(plain)) {
            mRoot->setProperty("okPass", true); // Se escribio.
        } else {
            mRoot->setProperty("failPass", true); // No se escribio.
        }
    }
}

// Update
void PIG::update()
{
    mUpdate = new Update();
    mUpdate->dbPath = dbPath;// No va.
    mUpdate->db = db; // No va.
    mUpdate->init(mRoot);
    emit showUpdateSIGNAL();

    connect(mUpdate, SIGNAL(updateCallFinderSIGNAL()), this, SLOT(finder()));
    connect(mUpdate, SIGNAL(updateErrorDbSIGNAL()), this, SLOT(errorDb()));
}

// Finder
void PIG::finder()
{
    delete mUpdate;

    QFileInfo file(dbPath);
    if (!file.isFile()) {
        errorDb();
    } else {
        if (db.open()) {
            QSqlQuery qry;
            qry.prepare("SELECT DbVersion, BinVersion, Release, Category, NCategory, Pornstar, NPornstar FROM PxData, FiltersData");
            if (!qry.exec()) {
                db.close();
                errorDb();
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

                mRoot->setProperty("dataBaseVersion", strLocalDbVersion);
                mRoot->setProperty("binaryVersion", strLocalBinVersion);
                mRoot->setProperty("release", strLocalRelease);
                mRoot->setProperty("categoryList", categoryList);
                mRoot->setProperty("nCategoryList", nCategoryList);
                mRoot->setProperty("pornstarList", pornstarList);
                mRoot->setProperty("nPornstarList", nPornstarList);
            }
        } else {
            errorDb();
        }
    }
    emit showFinderSIGNAL();
}

void PIG::findDb(const QString inputText, QString category, QString pornstar, int offset, bool init)
{
    if (!db.open()) {
        errorDb();
    } else {
        row = 0;
       _list.clear();
        QString strOffset = QString::number(offset);
        QSqlQuery qry;
            qry.prepare( "SELECT Title, Pornstar, Quality, Collaborator, Category, UrlCover, UrlPoster, UrlPreview, Torrent FROM Films WHERE Title LIKE '%"+inputText+"%' AND Category LIKE '%"+category+"%' AND Pornstar LIKE '%"+pornstar+"%' ORDER BY Title ASC LIMIT 1000 OFFSET '"+strOffset+"'" );
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
                QString _collaborator = qry.value(3).toString();
                QString _category = qry.value(4).toString();
                QString _urlCover = qry.value(5).toString();
                QString _urlPoster = qry.value(6).toString();
                QString _urlPreview = qry.value(7).toString();
                QString _torrent = qry.value(8).toString();
                _list << _title << _pornstars << _quality << _collaborator << _category << _urlCover << _urlPoster << _urlPreview << _torrent << inputText << category << pornstar;
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

void PIG::getTorrent(QString hostTorrent, QString urlTorrent, QString scenneID)
{
    QString fileTorrent = urlTorrent.replace("/torrent/", "", Qt::CaseSensitive);
    //TcpSocket s; // TODO: Instanciar el socket desde aca no de .h.
    mSocket.host = hostTorrent;
    mSocket.url = urlTorrent;
    mSocket.path = tmpPath;
    mSocket.fileName = fileTorrent;
    mSocket.doConnect();
    //mSocket.close();
    connect(&mSocket, SIGNAL(fileWrited(QString)), this, SLOT(torrentManager(QString)));
}

void PIG::torrentManager(QString fileTorrent)
{
    qDebug() << "FILE_TORRENT: " << fileTorrent;
    //Torrent t;
    mTorrent.download();//(tmpPath, fileTorrent);
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
    mPlayer->close(); // FIXME: Si salgo del reproductor antes de que reprodusca se rompe.
    delete mPlayer;

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
    if(mRoot) connect(mRoot, SIGNAL(passwordManager(QString, bool, bool)), this, SLOT(passwordManager(QString, bool, bool)));
    if(mRoot) connect(mRoot, SIGNAL(quit()), this, SLOT(quit()));

    passwordManager("", true, false);
}

void PIG::quit()
{
   exit(0);
}
