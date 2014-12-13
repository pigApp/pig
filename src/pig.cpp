#include "pig.h"
#include "password.h"

#include <QTextStream>
#include <QDir>
#include <QFile>

PIG::PIG(QWidget *parent) : QWidget(parent), mRoot(0)
{
    mUpdate = NULL;
    mTorrent = NULL;
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
        connect (mRoot, SIGNAL(signal_qml_preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool)), this,
                        SLOT(preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool)));
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
           emit signal_ret_password(true);
       else
           update_handler();
       file.close();
    } else if (check) {
        Password mPassword;
        if (mPassword.check(&plain))
            update_handler();
        else
            emit signal_ret_password();
    } else if (write) {
        Password mPassword;
        if (mPassword.write(&plain))
            emit signal_ret_password(false, true);
        else
            emit signal_ret_password();
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
            const QStringList totalCategories = query.value(4).toString().split(",");
            QStringList pornstars = query.value(5).toString().split(",");
            const QStringList totalPornstars = query.value(6).toString().split(",");
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
            mRoot->setProperty("totalCategories", totalCategories);
            mRoot->setProperty("pornstars", pornstars);
            mRoot->setProperty("totalPornstars", totalPornstars);

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
                file.rename(init, tmp+"init.trash");
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
                          const int id, const bool success, const bool abort)
{
    if (!target.isEmpty()) {
        mSocket[id] = new TcpSocket();
        mSocket[id]->host = host;
        mSocket[id]->urls << url;
        mSocket[id]->target = target;
        mSocket[id]->id = id;
        mSocket[id]->request = "PREVIEW";
        mSocket[id]->start();
        connect (mSocket[id], SIGNAL(signal_ret_preview(const QString, const QString, const QString, const QString, const int, const bool, const bool)),this,
                              SLOT(preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool)));
    } else if (target.isEmpty() && !abort) {
        emit signal_ret__preview(path, id, success);
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
    } else {
        mTorrent->deleteLater();
        mTorrent = NULL;
    }
}

//FIND
void PIG::find(const QString inputUser, const QString pornstar, const QString category,
               const QString quality, const QString full, const int offset, const bool init)
{
    if (db.open()) {
        QSqlQuery query;
            query.prepare("SELECT Cas, Category, Quality, Full, HostPreview, UrlPreview, FilePreview, HostCover, UrlFrontCover, UrlBackCover, Torrent \
                           FROM Films WHERE Title LIKE '%"+inputUser+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE '%"+category+"%' \
                           AND Quality LIKE '%"+quality+"%' AND Full LIKE '%"+full+"%' ORDER BY Title ASC LIMIT 1000 OFFSET '"+QString::number(offset)+"'");
        if (!query.exec()) {
            db.close();
            error_database();
        } else {
            if (init) {
                query.last();
                mRoot->setProperty("total_films", query.at()+1);
                query.first();
                query.previous();
            }
            int i = 0;
            QStringList dataFilms;
            for (i=0; query.next() && i<5; i++) {
                const QString strCast = query.value(0).toString();
                const QString strCategories = query.value(1).toString();
                const QString strQuality = query.value(2).toString();
                const QString strFull = query.value(3).toString();
                const QString strHostPreview = query.value(4).toString();
                const QString strUrlPreview = query.value(5).toString();
                const QString strFilePreview = query.value(6).toString();
                const QString strHostCover = query.value(7).toString();
                const QString strUrlFrontCover = query.value(8).toString();
                const QString strUrlBackCover = query.value(9).toString();
                const QString strTorrent = query.value(10).toString();
                dataFilms << strCast << strCategories << strQuality << strFull << strHostPreview << strUrlPreview << strFilePreview
                          << strHostCover << strUrlFrontCover << strUrlBackCover << strTorrent;
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
