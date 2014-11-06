#include "pig.h"

#include <QTextStream>
#include <QDir>
#include <QFile>

PIG::PIG(QWidget *parent) : QWidget(parent), mRoot(0)
{
    mSocket = NULL;
    mTorrent = NULL;
    mUpdate = NULL;
    mPassword = new Password();

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
    if (mSocket !=NULL) {
        for (int i=0; i<5; i++)
            delete mSocket[i];
        delete[] mSocket;
    }

    //cleanUp();
    mRoot->disconnect(this);
    exit(0);
}

void PIG::set_root_object(QObject *root)
{
    if (mRoot != 0)
        mRoot->disconnect(this);
    mRoot = root;

    if (mRoot) connect (mRoot, SIGNAL(password_handle_qml_signal(const QString, const bool)), this, SLOT(password_handle(const QString, const bool)));
    if (mRoot) connect (mRoot, SIGNAL(find_qml_signal(const QString, const QString, const QString, const QString, const QString, const int, const bool)),
                                      this, SLOT(find(const QString, const QString, const QString, const QString, const QString, const int, const bool)));
    if (mRoot) connect (mRoot, SIGNAL(preview_handle_qml_signal(const QString, const QString, const QString, const QString, const int, const bool, const bool)),
                                      this, SLOT(preview_handle(const QString, const QString, const QString, const QString, const int, const bool, const bool)));
    if (mRoot) connect (mRoot, SIGNAL(torrent_handle_qml_signal(const QString, int, bool)), this, SLOT(torrent_handle(const QString, int, bool)));
    if (mRoot) connect (mRoot, SIGNAL(quit_qml_signal()), this, SLOT(quit()));
}

//PASSWORD
void PIG::password_handle(const QString plain, const bool write)
{
    if (plain.isEmpty()) {
        if (mPassword->require())
            emit require_password_signal();
        else
            QTimer::singleShot(10, this, SLOT(update_handle()));
    } else if (!plain.isEmpty() && !write) {
        if (mPassword->success(&plain))
            update_handle();
        else
            emit fail_password_signal();
    } else {
        if (mPassword->write(&plain))
            emit success_password_signal();
        else
            emit fail_password_signal();
    }
}

//UPDATE
void PIG::update_handle()
{
    delete mPassword;
    mPassword = NULL;

    mUpdate = new Update();
#ifdef _WIN32
    const QString target = "C:/PIG/.pig/db.sqlite";
#else
    const QString target = QDir::homePath()+"/.pig/db.sqlite";
#endif
    QFile file(target);
    if (file.exists()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(target);

        emit show_update_signal();//

        mUpdate->_root = mRoot;
        mUpdate->db = &db;
        mUpdate->start();

        connect (mUpdate, SIGNAL(forward_signal()), this, SLOT(start_pig()));
        connect (mUpdate, SIGNAL(fail_database_signal()), this, SLOT(error_database()));
        connect (mRoot, SIGNAL(skip_qml_signal()), this, SLOT(start_pig()));
        connect (mRoot, SIGNAL(get_files_qml_signal()), mUpdate, SLOT(get_files()));
    } else {
        error_database();
    }
}

//START
void PIG::start_pig()
{
    delete mUpdate;
    mUpdate = NULL;

    if (db.open()) {
        QSqlQuery query;
        query.prepare("SELECT DatabaseVersion, BinaryVersion, Release, Category, NCategory, Pornstar, NPornstar FROM PigData, FiltersData");
        if (!query.exec()) {
            db.close();
            error_database();
        } else {
            query.next();
            const int currentDatabaseVersion = query.value(0).toInt();
            const int currentBinaryVersion = query.value(1).toInt();
            const int currentRelease = query.value(2).toInt();
            QStringList categoryList = query.value(3).toString().split(",");
            const QStringList nCategoryList = query.value(4).toString().split(",");
            QStringList pornstarList = query.value(5).toString().split(",");
            const QStringList nPornstarList = query.value(6).toString().split(",");
            db.close();

            const QString strCurrentDatabaseVersion = QString::number(currentDatabaseVersion);
            const QString strCurrentBinaryVersion = QString::number(currentBinaryVersion);
            const QString strCurrentRelease= QString::number(currentRelease);

            categoryList.prepend(QString::number(categoryList.count()));
            pornstarList.prepend(QString::number(pornstarList.count()));

            mRoot->setProperty("databaseVersion", strCurrentDatabaseVersion);
            mRoot->setProperty("binaryVersion", strCurrentBinaryVersion);
            mRoot->setProperty("release", strCurrentRelease);
            mRoot->setProperty("categoryList", categoryList);
            mRoot->setProperty("nCategoryList", nCategoryList);
            mRoot->setProperty("pornstarList", pornstarList);
            mRoot->setProperty("nPornstarList", nPornstarList);

#ifdef _WIN32
    const QString init = "C:/PIG/.pig/.init";
    const QString news = "C:/PIG/.pig/news.txt";
    const QString tmp = "C:/PIG/.pig/tmp/";
#else
    const QString init = QDir::homePath()+"/.pig/.init";
    const QString news = QDir::homePath()+"/.pig/news";
    const QString tmp = QDir::homePath()+"/.pig/tmp/";
#endif

            QFile file(init);
            if (file.exists()) {
                file.remove();
                file.setFileName(news);
                if (file.exists())
                    file.remove();
                emit show_welcome_signal();
            } else {
                file.setFileName(news);
                if (file.exists()) {
                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                    bool head = true;
                    QString binaryNews;
                    QString databaseNews;
                    QTextStream in(&file);
                    while (!in.atEnd()) {
                        const QString line = in.readLine();
                        if (line.isEmpty())
                            head = false;
                        if (head) {
                            binaryNews.append(line+"\n");
                        } else {
                            if(!line.isEmpty())
                            databaseNews.append(line+"\n");
                        }
                    }
                    file.remove();
                    emit show_news_signal(binaryNews, databaseNews);
                } else {
                    emit show_finder_signal();
                }
            }
            const QDir dir(tmp);
            if (!dir.exists())
                dir.mkdir(tmp);

            mSocket = new TcpSocket*[5];
            for (int i=0; i<5; i++)
                mSocket[i] = new TcpSocket();

            mTorrent = new Torrent();
            mTorrent->_root = mRoot;
        }
    } else {
        error_database();
    }
}

//FIND
void PIG::find(const QString input, const QString pornstar, const QString category, const QString quality, const QString full, const int offset, const bool init)
{
    QStringList _list;
    int row = 0;

    if (!db.open()) {
        error_database();
    } else {
        _list.clear();
        const QString strOffset = QString::number(offset);
        QSqlQuery query;
            query.prepare("SELECT Title, Cas, Category, Quality, Full, HostPreview, UrlPreview, FilePreview, HostPosterCover, UrlPoster, UrlCover, Torrent FROM Films WHERE Title LIKE '%"+input+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE '%"+category+"%' AND Quality LIKE '%"+quality+"%' AND Full LIKE '%"+full+"%' ORDER BY Title ASC LIMIT 1000 OFFSET '"+strOffset+"'");
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
            for (row=0; query.next() && row<5; row++) {
                const QString _title = query.value(0).toString();
                const QString _cast = query.value(1).toString();
                const QString _category = query.value(2).toString();
                const QString _quality = query.value(3).toString();
                const QString _full = query.value(4).toString();
                const QString _hostPreview = query.value(5).toString();
                const QString _urlPreview = query.value(6).toString();
                const QString _filePreview = query.value(7).toString();
                const QString _hostPosterCover = query.value(8).toString();
                const QString _urlPoster = query.value(9).toString();
                const QString _urlCover = query.value(10).toString();
                const QString _torrent = query.value(11).toString();
                _list << _title << _cast << _category << _quality << _full << _hostPreview << _urlPreview << _filePreview << _hostPosterCover << _urlPoster << _urlCover << _torrent;
            }
            db.close();

            if (!query.last() && init) {
                emit no_result_signal();
            } else {
                if (init)
                    emit show_output_signal(row, _list);
                else
                    emit success_update_list_signal(row, _list);
            }
        }
    }
}

//PREVIEW
void PIG::preview_handle(const QString host, const QString url, const QString path, const QString file, const int id, const bool success, const bool fail)
{
    if (!success && !fail) {
        qDebug() << "//// ID: " << id; //
        mSocket[id]->host = host;
        mSocket[id]->url = url;
        mSocket[id]->file = file;
        mSocket[id]->id = id;
        mSocket[id]->call = "PREVIEW";
        mSocket[id]->start();
    } else {
        if (success)
            emit success_preview_signal(path, file, id);
        else if (fail)
            emit fail_preview_signal(id);

    }
    connect (mSocket[id], SIGNAL(ret_preview_signal(const QString, const QString, const QString, const QString, const int, const bool, const bool)),
                                 this, SLOT(preview_handle(const QString, const QString, const QString, const QString, const int, const bool, const bool)));
}

//TORRENT
void PIG::torrent_handle(const QString magnet, int scene, bool abort)
{
    //cleanUp();

    if (!abort) {
        mTorrent->scene = scene;
        mTorrent->start(&magnet);
    } else {
        mTorrent->stop();
    }
    connect (mTorrent, SIGNAL(sandbox_signal(const QString, bool, bool, bool)), this, SLOT(player_handle(const QString, bool, bool, bool)));
}

//PLAYER
void PIG::player_handle(const QString absoluteFilePath, bool sandbox, bool fileReady, bool abort)
{
    if (!abort) {
        if (sandbox) {
            mPlayer = new VideoPlayer();
            mPlayer->_torrent = mTorrent;
            mPlayer->sandbox(&absoluteFilePath);
            mTorrent->_player = mPlayer;

            connect (mPlayer, SIGNAL(success_file_signal(const QString, bool, bool, bool)), this, SLOT(player_handle(const QString, bool, bool, bool)));
            connect (mPlayer, SIGNAL(close_player_signal(const QString, bool, bool, bool)), this, SLOT(player_handle(const QString, bool, bool, bool)));
            connect (mPlayer, SIGNAL(quit_signal()), this, SLOT(quit()));

            emit checking_file_signal();
        }
        if (fileReady) {
            mTorrent->toPlayer = true;
            mPlayer->showFullScreen();
            this->hide();
            emit success_file_signal();
        }
    } else {
        this->show();
        emit abort_torrent_signal();
        mPlayer->close();
        delete mPlayer;
    }
}

//CLEANUP
void PIG::cleanUp()
{
    #ifdef _WIN32
        const QString target = "C:/PIG/.pig/tmp/";
    #else
        const QString target = QDir::homePath()+"/.pig/tmp/";
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

//ERROR_DB
void PIG::error_database()
{
    emit show_errorDatabase_signal();
}

//QUIT
void PIG::quit()
{
    this->~PIG();
}
