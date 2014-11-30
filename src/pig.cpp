#include "pig.h"
#include "password.h"

#include <QTextStream>
#include <QDir>
#include <QFile>

PIG::PIG(QWidget *parent) : QWidget(parent), mRoot(0)
{
    mUpdate = NULL;
    mTorrent = NULL;
    mPlayer = NULL;
    for (int i=0; i<5; i++)
        mSocket[i] = NULL;

    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
}

PIG::~PIG()
{
    if (mUpdate != NULL)
        delete mUpdate;
    if (mPlayer != NULL)
        delete mPlayer;
    if (mTorrent != NULL)
        delete mTorrent;
    for (int i=0; i<5; i++)
        if (mSocket[i] != NULL)
            delete mSocket[i];
    mRoot->disconnect(this);
    exit(0);
}

void PIG::set_root_object(QObject *root)
{
    if (mRoot != NULL)
        mRoot->disconnect(this);
    mRoot = root;
    if (mRoot != NULL) {
        connect (mRoot, SIGNAL(signal_qml_password_handler(const QString, const bool, const bool, const bool)), this,
                        SLOT(password_handler(const QString, const bool, const bool, const bool)));
        connect (mRoot, SIGNAL(signal_qml_find(const QString, const QString, const QString, const QString, const QString, const int, const bool)), this,
                        SLOT(find(const QString, const QString, const QString, const QString, const QString, const int, const bool)));
        connect (mRoot, SIGNAL(signal_qml_preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool, const bool)), this,
                        SLOT(preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool, const bool)));
        connect (mRoot, SIGNAL(signal_qml_torrent_handler(const QString, const int, const bool)), this, SLOT(torrent_handler(const QString, const int, const bool)));
        connect (mRoot, SIGNAL(signal_qml_quit()), this, SLOT(quit()));
    }
}

//PASSWORD
void PIG::password_handler(const QString plain, const bool require, const bool check, const bool write)
{
    if (require) {
#ifdef __linux__
    const QString target = QDir::homePath()+"/.pig/.pd";
#else
    const QString target = "C:/PIG/.pig/.pd";
#endif
       QFile file;
       if (file.exists(target))
           emit signal_require_password();
       else
           update_handler();
       file.close();
    } else if (check) {
        Password mPassword;
        if (mPassword.check(&plain))
            update_handler();
        else
            emit signal_fail_password();
    } else if (write) {
        Password mPassword;
        if (mPassword.write(&plain))
            emit signal_success_password();
        else
            emit signal_fail_password();
    }
}

//UPDATE
void PIG::update_handler()
{
    //cleanup();
#ifdef __linux__
    const QString target = QDir::homePath()+"/.pig/db.sqlite";
#else
    const QString target = "C:/PIG/.pig/db.sqlite";
#endif
    QFile file;
    if (file.exists(target)) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(target);
        mUpdate = new Update();
        mUpdate->_root = &mRoot;
        mUpdate->db = &db;
        mUpdate->start();
        connect (mUpdate, SIGNAL(signal_continue()), this, SLOT(start_pig()));
        connect (mUpdate, SIGNAL(signal_fail_database()), this, SLOT(error_database()));
        emit signal_show_update();
    } else {
        error_database();
    }
}

//START
void PIG::start_pig()
{
    mUpdate->deleteLater();
    mUpdate = NULL;

    if (db.open()) {
        QSqlQuery query;
        query.prepare("SELECT Binary, Release, Database, Categories, NCategories, Pornstars, NPornstars FROM PigData, FiltersData");
        if (!query.exec()) {
            db.close();
            error_database();
        } else {
            query.next();
            const int binary = query.value(0).toInt();
            const int release = query.value(1).toInt();
            const int database = query.value(2).toInt();
            QStringList categories = query.value(3).toString().split(",");
            const QStringList nCategories = query.value(4).toString().split(",");
            QStringList pornstars = query.value(5).toString().split(",");
            const QStringList nPornstars = query.value(6).toString().split(",");
            db.close();

            const QString strBinary = QString::number(binary);
            const QString strRelease= QString::number(release);
            const QString strDatabase = QString::number(database);
            categories.prepend(QString::number(categories.count()));
            pornstars.prepend(QString::number(pornstars.count()));

            mRoot->setProperty("binary", strBinary);
            mRoot->setProperty("release", strRelease);
            mRoot->setProperty("database", strDatabase);
            mRoot->setProperty("categories", categories);
            mRoot->setProperty("nCategories", nCategories);
            mRoot->setProperty("pornstars", pornstars);
            mRoot->setProperty("nPornstars", nPornstars);

#ifdef __linux__
    const QString init = QDir::homePath()+"/.pig/.init";
    const QString news = QDir::homePath()+"/.pig/news";
    const QString tmp = QDir::homePath()+"/.pig/tmp/";
#else
    const QString init = "C:/PIG/.pig/.init";
    const QString news = "C:/PIG/.pig/news.txt";
    const QString tmp = "C:/PIG/.pig/tmp/";
#endif
            QFile file;
            if (file.exists(init)) {
                //file.rename(init, tmp+"init.trash");
                mRoot->setProperty("init", true);
            }
            file.setFileName(news);
            if (file.exists()) {
                file.open(QIODevice::ReadOnly|QIODevice::Text);
                bool binaryData = true;
                QString binaryNews;
                QString databaseNews;
                QTextStream in(&file);
                while (!in.atEnd()) {
                    const QString line = in.readLine();
                    if (!line.isEmpty())
                        if (binaryData)
                            binaryNews.append(line+"\n");
                        else
                            databaseNews.append(line+"\n");
                    else
                        binaryData = false;
                }
                file.rename(tmp+"news.trash");
                emit signal_show_news(binaryNews, databaseNews);
            } else {
                emit signal_show_finder();
            }
        }
    } else {
        error_database();
    }
}

//PREVIEW
void PIG::preview_handler(const QString host, const QString url, const QString path, const QString target,
                          const int id, const bool success, const bool fail, const bool abort)
{
    if (!success && !fail && !abort) {
        mSocket[id] = new TcpSocket();
        mSocket[id]->host = host;
        mSocket[id]->urls << url;
        mSocket[id]->target = target;
        mSocket[id]->id = id;
        mSocket[id]->request = "PREVIEW";
        mSocket[id]->start();
        connect (mSocket[id], SIGNAL(signal_preview_ret(const QString, const QString, const QString, const QString, const int, const bool, const bool, const bool)),this,
                              SLOT(preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool, const bool)));
    } else if (success || fail) {
        if (success)
            emit signal_success_preview(path, id);
        else if (fail)
            emit signal_fail_preview(id);
        mSocket[id]->deleteLater();
        mSocket[id] = NULL;
    } else if (abort) {
        mSocket[id]->abortedPreview = true;
        mSocket[id]->deleteLater();
        mSocket[id] = NULL;
    }
}

//TORRENT
void PIG::torrent_handler(const QString magnet, const int scene, const bool abort)
{
    //cleanup();
    if (!abort) {
        mTorrent = new Torrent(NULL, &magnet);
        mTorrent->_root = &mRoot;
        mTorrent->scene = scene;
        connect (mTorrent, SIGNAL(signal_sandbox(const QString, const bool, const bool, const bool)), this,
                           SLOT(player_handler(const QString, const bool, const bool, const bool)));
    } else {
        mTorrent->deleteLater();
        mTorrent = NULL;
        emit signal_hide_torrent_information();
    }
}

//PLAYER
void PIG::player_handler(const QString absoluteFilePath, const bool sandbox, const bool fileReady, const bool close)
{
    if (!close) {
        if (sandbox) {
            mPlayer = new VideoPlayer(NULL, &absoluteFilePath);
            mPlayer->_torrent = mTorrent;
            mTorrent->_player = &mPlayer;
            connect (mPlayer, SIGNAL(signal_file_ready(const QString, const bool, const bool, const bool)), this,
                              SLOT(player_handler(const QString, const bool, const bool, const bool)));
            connect (mPlayer, SIGNAL(signal_close_player(const QString, const bool, const bool, const bool)), this,
                              SLOT(player_handler(const QString, const bool, const bool, const bool)));
            connect (mPlayer, SIGNAL(signal_quit()), this, SLOT(quit()));
            emit signal_checking_file();
        }
        if (fileReady) {
            mTorrent->toPlayer = true;
            mPlayer->showFullScreen();
            hide();
            emit signal_file_ready();
        }
    } else {
        show();
        delete mPlayer;
        mPlayer = NULL;
        torrent_handler("", 0, true);
    }
}

//FIND
void PIG::find(const QString input, const QString pornstar, const QString category,
               const QString quality, const QString full, const int offset, const bool init)
{
    if (db.open()) {
        QSqlQuery query;
            query.prepare("SELECT Title, Cas, Category, Quality, Full, HostPreview, UrlPreview, FilePreview, HostPosterCover, UrlPoster, UrlCover, Torrent \
                           FROM Films WHERE Title LIKE '%"+input+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE '%"+category+"%' \
                           AND Quality LIKE '%"+quality+"%' AND Full LIKE '%"+full+"%' ORDER BY Title ASC LIMIT 1000 OFFSET '"+QString::number(offset)+"'");
        if (!query.exec()) {
            db.close();
            error_database();
        } else {
            if (init) {
                query.last();
                mRoot->setProperty("totalFilms", query.at()+1);
                query.first();
                query.previous();
            }
            int i = 0;
            QStringList dataFilms;
            for (i=0; query.next() && i<5; i++) {
                const QString strTitle = query.value(0).toString();
                const QString strCast = query.value(1).toString();
                const QString strCategories = query.value(2).toString();
                const QString strQuality = query.value(3).toString();
                const QString strFull = query.value(4).toString();
                const QString strHostPreview = query.value(5).toString();
                const QString strUrlPreview = query.value(6).toString();
                const QString strFilePreview = query.value(7).toString();
                const QString strHostPosterCover = query.value(8).toString();
                const QString strUrlPoster = query.value(9).toString();
                const QString strUrlCover = query.value(10).toString();
                const QString strTorrent = query.value(11).toString();
                dataFilms << strTitle << strCast << strCategories << strQuality << strFull << strHostPreview << strUrlPreview << strFilePreview
                          << strHostPosterCover << strUrlPoster << strUrlCover << strTorrent;
            }
            db.close();
            if (!query.last() && init) {
                emit signal_ret_db(0, dataFilms, false);
            } else {
                if (init)
                    emit signal_ret_db(i, dataFilms, false);
                else
                    emit signal_ret_db(i, dataFilms, true);
            }
        }
    } else {
        error_database();
    }
}

//CLEANUP
void PIG::cleanup()
{
#ifdef __linux__
    const QString target = QDir::homePath()+"/.pig/tmp/";
#else
    const QString target = "C:/PIG/.pig/tmp/";
#endif
    QDir dir(target);
    if (!dir.exists()) {
        dir.mkdir(target);
    } else {
        dir.setFilter(QDir::NoDotAndDotDot|QDir::Files);
        foreach(QString dirItem, dir.entryList())
            dir.remove(dirItem);

        dir.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
        foreach(QString dirItem, dir.entryList()) {
            QDir subDir(dir.absoluteFilePath(dirItem));
            subDir.removeRecursively();
        }
    }
}

//ERROR_DB
void PIG::error_database()
{
    emit signal_show_errorDatabase();
}

//QUIT
void PIG::quit()
{
    this->~PIG();
}
