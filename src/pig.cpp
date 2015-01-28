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

//QUIT
void PIG::quit()
{
    this->~PIG();
}

//QML_GUI
void PIG::set_root_object(QObject *root)
{
    if (mRoot != NULL)
        mRoot->disconnect(this);
    mRoot = root;
    if (mRoot != NULL) {
        connect (mRoot, SIGNAL(sig_qml_password_handler(const bool, const QString, const bool, const bool)), this,
            SLOT(password_handler(const bool, const QString, const bool, const bool)));
        connect (mRoot, SIGNAL(sig_qml_find(const QString, const QString, const QString, const QString, const QString)), this,
            SLOT(find(const QString, const QString, const QString, const QString, const QString)));
        connect (mRoot, SIGNAL(sig_qml_preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool)), this,
            SLOT(preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool)));
        connect (mRoot, SIGNAL(sig_qml_torrent_handler(const QString, const int, const bool)), this, SLOT(torrent_handler(const QString, const int, const bool)));
        connect (mRoot, SIGNAL(sig_qml_quit()), this, SLOT(quit()));
    }
}

//PASSWORD
void PIG::password_handler(const bool require, const QString plain, const bool check, const bool write)
{
    if (require) {
#ifdef __linux__
    const QString target = QDir::homePath()+"/.pig/.pd";
#else
    const QString target = "C:/PIG/.pig/.pd";
#endif
       QFile file;
       if (file.exists(target))
           emit sig_ret_password(true);
       else
           update_handler();
       file.close();
    } else if (check) {
        Password mPassword;
        if (mPassword.check(&plain))
            update_handler();
        else
            emit sig_ret_password();
    } else if (write) {
        Password mPassword;
        if (mPassword.write(&plain))
            emit sig_ret_password(false, true);
        else
            emit sig_ret_password();
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
        connect (mRoot, SIGNAL(sig_qml_update_get()), mUpdate, SLOT(user_confirmation()));
        connect (mRoot, SIGNAL(sig_qml_update_skip()), this, SLOT(start()));
        connect (mUpdate, SIGNAL(sig_continue()), this, SLOT(start()));
        connect (mUpdate, SIGNAL(sig_fail_database()), this, SLOT(db_err()));
        emit sig_show_update();
    } else {
        db_err();
    }
}

//START
void PIG::start()
{
    mUpdate->deleteLater();
    mUpdate = NULL;

    if (db.open()) {
        QSqlQuery query;
        query.prepare("SELECT Binary, Release, Database, Categories, NCategories, Pornstars, NPornstars FROM PigData, FiltersData");
        if (!query.exec()) {
            db.close();
            db_err();
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
            mRoot->setProperty("n_categories", nCategories);
            mRoot->setProperty("pornstars", pornstars);
            mRoot->setProperty("n_pornstars", nPornstars);

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
                emit sig_show_news(binaryNews, databaseNews);
            } else {
                emit sig_show_finder();
            }
        }
    } else {
        db_err();
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
        connect (mSocket[id], SIGNAL(sig_ret_preview(const QString, const QString, const QString, const QString, const int, const bool, const bool)),this,
            SLOT(preview_handler(const QString, const QString, const QString, const QString, const int, const bool, const bool)));
    } else if (target.isEmpty() && !abort) {
        emit sig_ret_preview(path, id, success);
        mSocket[id]->deleteLater();
        mSocket[id] = NULL;
    } else if (abort) {
        mSocket[id]->abortPreview = true;
        mSocket[id]->deleteLater();
        mSocket[id] = NULL;
    }
}

//TORRENT
void PIG::torrent_handler(const QString url, const int scene, const bool abort)
{
    //cleanup();
    if (!abort) {
        mTorrent = new Torrent(NULL, &url);
        mTorrent->_root = &mRoot;
        mTorrent->scene = scene+1; //TODO: ver si esta bien sumarle 1 a scene.
    } else {
        mTorrent->deleteLater();
        mTorrent = NULL;
    }
}

//FIND
void PIG::find(const QString userInput, const QString pornstar, const QString category,
    const QString quality, const QString full)
{
    if (db.open()) {
        QSqlQuery query;
        query.prepare("SELECT Title, Cas, Category, Quality, Time, Full, HostPreview, UrlPreview, FilePreview, HostCover, UrlFrontCover, UrlBackCover, Torrent \
            FROM Films WHERE Title LIKE '%"+userInput+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE '%"+category+"%' \
            AND Quality LIKE '%"+quality+"%' AND Full LIKE '%"+full+"%' ORDER BY Title ASC LIMIT 1000");
        if (!query.exec()) {
            db.close();
            db_err();
        } else {
            int nFilms = 0;
            QStringList dataFilms;
            for (int i=0; query.next(); i++) {
                const QString strTitle = query.value(0).toString();
                const QString strCast = query.value(1).toString();
                const QString strCategories = query.value(2).toString();
                const QString strQuality = query.value(3).toString();
                const QString strTime = query.value(4).toString();
                const QString strFull = query.value(5).toString();
                const QString strHostPreview = query.value(6).toString();
                const QString strUrlPreview = query.value(7).toString();
                const QString strFilePreview = query.value(8).toString();
                const QString strHostCover = query.value(9).toString();
                const QString strUrlFrontCover = query.value(10).toString();
                const QString strUrlBackCover = query.value(11).toString();
                const QString strTorrent = query.value(12).toString();
                dataFilms << strTitle << strCast << strCategories << strQuality << strTime << strFull << strHostPreview << strUrlPreview
                    << strFilePreview << strHostCover << strUrlFrontCover << strUrlBackCover << strTorrent;
            }
            db.close();
            if (!query.last()) {
                emit sig_ret_db(nFilms, dataFilms);
            } else {
                nFilms = query.at()+1;
                emit sig_ret_db(nFilms, dataFilms);
            }
        }
    } else {
        db_err();
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

//DB_ERROR
void PIG::db_err()
{
    emit sig_show_db_err();
}
