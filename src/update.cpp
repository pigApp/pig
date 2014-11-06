#include "update.h"

#include <QCryptographicHash>
#include <QTimer>

Update::Update(QObject *parent) : QObject(parent)
{
    newsAvailable = false;
    newBinaryAvailable = false;
    newDatabaseAvailable = false;
    databaseUpdated = false;

    mSocket = new TcpSocket();
    connect (mSocket, SIGNAL(success_version_signal(const QString)), this, SLOT(evaluate(const QString)));
    connect (mSocket, SIGNAL(success_file_signal(const QString, const QString)), this, SLOT(integrityFile(const QString, const QString)));
    connect (mSocket, SIGNAL(fail_socket_signal()), this, SLOT(error()));

    _root = NULL;
}

Update::~Update()
{
    if (_root != NULL)
        _root->disconnect(this);
    mSocket->deleteLater();
}

void Update::start()
{
    _root->setProperty("showNetwork", true);
    
    if (db->open()) {
        QSqlQuery query;
        query.prepare("SELECT BinaryVersion, DatabaseVersion, Release, Host, Url FROM PigData");
        if (!query.exec()) {
            db->close();
            emit fail_database_signal();
        } else {
            query.next();
            currentBinaryVersion = query.value(0).toInt();
            currentDatabaseVersion = query.value(1).toInt();
            currentRelease = query.value(2).toInt();
            const QString host = query.value(3).toString();
            const QString urls = query.value(4).toString();
            const QStringList urlList = urls.split(",");
            const int byte = sizeof(void*);
            QString url;

#ifdef _WIN32
    if (byte == 4)
        url = urlList[0];//2------------
    else
        url = urlList[1];//3------------
#else
    if (byte == 4)
        url = urlList[0];//0------------
    else
        url = urlList[1];//1------------
#endif
            db->close();
            get_version(&host, &url);
        }
    } else {
        emit fail_database_signal();
    }
}

void Update::get_version(const QString *const host, const QString *const url)
{
    mSocket->host = *host;
    mSocket->url = *url;
    mSocket->file = ".";
    mSocket->call = "VERSION";
    mSocket->start();
}

void Update::evaluate(const QString version)
{
    const QStringList last = version.split(",");

    if (last[3].toInt()+last[9].toInt() > currentBinaryVersion+currentRelease) {
        binaryHash = last[5];
        newBinaryAvailable = true;
        currentBinaryVersion = last[3].toInt();
        currentRelease = last[9].toInt();
    }
    if (last[6].toInt() > currentDatabaseVersion) {
        databaseHash = last[8];
        newDatabaseAvailable = true;
    }

    if (newBinaryAvailable || newDatabaseAvailable) {
        newsAvailable = true;
        _root->setProperty("showNetwork", false);
        _root->setProperty("status", "UPDATE AVAILABLE");
        _root->setProperty("requireConfirmation", true);
        hostFiles = last[0];
        newsUrl = last[1];
        newsHash = last[2];
        binaryUrl = last[4];
        databaseUrl = last[7];
    } else {
        _root->setProperty("showNetwork", false);
        emit forward_signal();
    }
}

void Update::get_files()
{
    _root->setProperty("showNetwork", true);
    _root->setProperty("status", "");
    _root->setProperty("requireConfirmation", false);

    if (newBinaryAvailable && !newDatabaseAvailable && !newsAvailable) {
        mSocket->host = hostFiles;
        mSocket->url = binaryUrl;
#ifdef _WIN32
    mSocket->file = "pig.exe";
#else
    mSocket->file = "pig";
#endif
        mSocket->call = "BIN";
        mSocket->start();
    } else if (newDatabaseAvailable && !newsAvailable) {
        mSocket->host = hostFiles;
        mSocket->url = databaseUrl;
        mSocket->file = "db.sqlite";
        mSocket->call = "DB";
        mSocket->start();
    } else if (newsAvailable) {
        mSocket->host = hostFiles;
        mSocket->url = newsUrl;
        mSocket->file = "news.txt";
        mSocket->call = "NEWS";
        mSocket->start();
    }
}

void Update::integrityFile(QString path, QString file)
{
    QFile target(path+file);
    target.open(QIODevice::ReadOnly);
    QByteArray raw = target.readAll();
    const QString targetHash = QCryptographicHash::hash(raw, QCryptographicHash::Md5).toHex();
    target.close();
    raw.clear();

    if (file == "news.txt") {
        if (targetHash == newsHash)
            replace(&path, &file);
        else
           error();
    } else if (file == "pig" || file == "pig.exe") {
        if (targetHash == binaryHash)
            replace(&path, &file);
        else
            error();
    } else if (file == "db.sqlite") {
        if (targetHash == databaseHash)
            replace(&path, &file);
        else
            error();
    } else if (file == "lib*") {           // TODO: LIBS
        if (targetHash == databaseHash)
            replace(&path, &file);
        else
            error();
    }
}

void Update::replace(QString *path, QString *file)
{
    if (newBinaryAvailable && !newDatabaseAvailable && !newsAvailable) {
        newBinaryAvailable = false;
        _root->setProperty("status", "");
        _root->setProperty("showNetwork", false);
#ifdef _WIN32
    updateProc = new QProcess(this);
    if (updateProc->startDetached("C:\\PIG\\.pig\\update.bat"))
        replace_binary_success(0);
    else
        replace_binary_success(1);
#else
    QString suManager;
    suManagerProc = new QProcess(this);
    suManagerProc->start("/bin/bash", QStringList() << "-c" << "ls /usr/bin/gksu");
    suManagerProc->waitForFinished(500);
    if (suManagerProc->exitCode() != 0) {
        suManagerProc->close();
        suManagerProc = new QProcess(this);
        suManagerProc->start("/bin/bash", QStringList() << "-c" << "ls /usr/bin/kdesu");
        suManagerProc->waitForFinished(500);
        if (suManagerProc->exitCode() == 0)
            suManager = "kdesu";
        suManagerProc->close();
    } else {
        suManagerProc->close();
        suManager = "gksu";
    }
    if (!suManager.isEmpty()) {
        updateProc = new QProcess(this);
        if (suManager == "gksu")
            updateProc->start("/bin/bash", QStringList() << "-c" << suManager+" -u root -m 'PIG UPDATE' 'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ ; chmod +x /usr/bin/pig'");
        else
            updateProc->start("/bin/bash", QStringList() << "-c" << suManager+" -u root -c 'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ ; chmod +x /usr/bin/pig'");
        connect (updateProc, SIGNAL(finished(int)), this, SLOT(replace_binary_success(int)));
    } else{
        replace_binary_success(5);
    }
#endif
    } else if (newDatabaseAvailable && !newsAvailable) {
#ifdef _WIN32
    const QString target = "C:/PIG/.pig/db.sqlite";
    const QString target_backup = "C:/PIG/.pig/tmp/db.sqlite~";
#else
    const QString target = QDir::homePath()+"/.pig/db.sqlite";
    const QString target_backup = QDir::homePath()+"/.pig/tmp/db.sqlite~";
#endif
        newDatabaseAvailable = false;
        QFile f(target_backup);
        if (f.exists())
            f.remove();
        f.setFileName(target);
        if (f.exists())
            f.rename(target_backup);
        if (QFile::copy(*path+*file, target)) {
            databaseUpdated = true;
            if (newBinaryAvailable)
                get_files();
            else
                emit forward_signal();
        } else {
            newBinaryAvailable = false;
            _root->setProperty("showNetwork", false);
            _root->setProperty("status", "");
            error();
        }
    } else if (newsAvailable) {
#ifdef _WIN32
    const QString target = "C:/PIG/.pig/news.txt";
#else
    const QString target = QDir::homePath()+"/.pig/news";
#endif
        QFile::copy(*path+*file, target);
        newsAvailable = false;
        get_files();
    }
}

void Update::replace_binary_success(int exitCode)
{
    if (exitCode != 5)
        updateProc->close();
#ifdef _WIN32
    if (exitCode == 0) {
        if (!databaseUpdated) {
            if (db->open()) {
                QSqlQuery query;
                query.prepare("UPDATE PigData SET BinaryVersion='"+QString::number(currentBinaryVersion)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(currentRelease)+"'");
                query.exec();
                db->close();
            }
        }
        exit(0);
    } else {
        _root->setProperty("status", "UPDATE FAILED");
        _root->setProperty("information", "TRY LATER");
        QTimer::singleShot(3000, this, SLOT(error()));
    }
#else
    if (exitCode == 0) {
        if (!databaseUpdated) {
            if (db->open()) {
                QSqlQuery query;
                query.prepare("UPDATE PigData SET BinaryVersion='"+QString::number(currentBinaryVersion)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(currentRelease)+"'");
                query.exec();
                db->close();
            }
        }
        _root->setProperty("status", "UPDATED");
        _root->setProperty("information", "RESTART PIG");
    } else if (exitCode == 5) {
        _root->setProperty("status", "UPDATE FAILED");
        _root->setProperty("information", "Install gksu/kdesu");
        QTimer::singleShot(10000, this, SLOT(error()));
    } else {
        _root->setProperty("status", "UPDATE FAILED");
        _root->setProperty("information", "TRY LATER");
        QTimer::singleShot(3000, this, SLOT(error()));
    }
#endif
}

void Update::error()
{
#ifdef _WIN32
    const QString target_news = "C:/PIG/.pig/news.txt";
#else
    const QString target_news = QDir::homePath()+"/.pig/news";
#endif
    QFile file_news(target_news);
    if (file_news.exists())
            file_news.remove();
    
#ifdef _WIN32
    const QString target = "C:/PIG/.pig/db.sqlite";
    const QString target_backup = "C:/PIG/.pig/tmp/db.sqlite~";
#else
    const QString target = QDir::homePath()+"/.pig/db.sqlite";
    const QString target_backup = QDir::homePath()+"/.pig/tmp/db.sqlite~";
#endif
    QFile file_backup(target_backup);
    if (databaseUpdated) {
        QFile file(target);
        file.remove();
        file_backup.rename(target);
    }
    emit forward_signal();
}
