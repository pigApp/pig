#include "update.h"

#include <QCryptographicHash>
#include <QTimer>
#ifdef _WIN32
    #include <windows.h>
#endif

Update::Update(QObject *parent) : QObject(parent)
{
    newsAvailable = false;
    newBinaryAvailable = false;
    newDatabaseAvailable = false;
    databaseUpdated = false;

    mSocket = new TcpSocket();
    connect(mSocket, SIGNAL(version_ready(QString)), this, SLOT(evaluate(QString)));
    connect(mSocket, SIGNAL(file_ready(QString, QString)), this, SLOT(integrityFile(QString, QString)));
    connect(mSocket, SIGNAL(error_socket()), this, SLOT(abort()));
}

Update::~Update()
{
    _root->disconnect(this);
    mSocket->deleteLater();
}

void Update::check()
{
    _root->setProperty("showNetwork", true);
    
    if (db->open()) {
        QSqlQuery qry;
        qry.prepare("SELECT BinaryVersion, DatabaseVersion, Release, Host, Url FROM PigData");
        if (!qry.exec()) {
            db->close();
            emit errorDatabaseSIGNAL();
        } else {
            qry.next();
            currentBinaryVersion = qry.value(0).toInt();
            currentDatabaseVersion = qry.value(1).toInt();
            currentRelease = qry.value(2).toInt();
            const QString host = qry.value(3).toString();
            const QString urls = qry.value(4).toString();
            const QStringList urlList = urls.split(",");
            const int arch = sizeof(void*);
            QString url;
#ifdef _WIN32
    if (arch != 8) // TODO: Confirmar si distingue arquitectura.
        url = urlList[2];
    else
        url = urlList[3];
#else
    if (arch != 8)
        url = urlList[0];
    else
        url = urlList[1];
#endif
            db->close();
            get_version(&host, &url);
        }
    } else {
        emit errorDatabaseSIGNAL();
    }
}

void Update::get_version(const QString *const host, const QString *const url)
{
    mSocket->host = *host;
    mSocket->url = *url;
    mSocket->file = ".";
    mSocket->order = "getVersion";
    mSocket->doConnect();
}

void Update::evaluate(QString version)
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
        _root->setProperty("status", "");
        emit forwardSIGNAL();
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
        mSocket->order = "getFile";
        mSocket->doConnect();
    } else if (newDatabaseAvailable && !newsAvailable) {
        mSocket->host = hostFiles;
        mSocket->url = databaseUrl;
        mSocket->file = "db.sqlite";
        mSocket->order = "getFile";
        mSocket->doConnect();
    } else if (newsAvailable) {
        mSocket->host = hostFiles;
        mSocket->url = newsUrl;
        mSocket->file = "news.txt";
        mSocket->order = "getFile";
        mSocket->doConnect();
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
           abort();
    } else if (file == "pig" || file == "pig.exe") {
        if (targetHash == binaryHash)
            replace(&path, &file);
        else
            abort();
    } else {
        if (targetHash == databaseHash)
            replace(&path, &file);
        else
            abort();
    }
}

void Update::replace(QString *path, QString *file)
{
    if (newBinaryAvailable && !newDatabaseAvailable && !newsAvailable) {
        newBinaryAvailable = false;
        _root->setProperty("status", "UPDATING");
#ifdef _WIN32
    const QString target = "C:/pig/bin/pig.exe";
    const QString updater = "C:/pig/bin/updater.exe";
    const QString updaterAguments = " "+*path+*file+" "+target;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess((TCHAR*)(updater.utf16()), (TCHAR*)(updaterAguments.utf16()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        _root->setProperty("showNetwork", false);
        _root->setProperty("status", "PERMISSION DENIED");
        _root->setProperty("information", "RESTART THE APPLICATION WITH ADMINITRATOR RIGHTS");
    } else {
        replace_binary_ready(0);
    }
#else
    updaterProc = new QProcess(this);                                                                    // TODO: Ver si QDir de la linea de abajo funciona.
    updaterProc->start("/bin/bash", QStringList() << "-c" << "gksu -u root -m 'PIG authorization to install update' 'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ ; chmod +x /usr/bin/pig'");
    connect(updaterProc, SIGNAL(finished(int)), this, SLOT(replace_binary_ready(int)));
#endif
    } else if (newDatabaseAvailable && !newsAvailable) {
#ifdef _WIN32
    const QString target = "C:/PIG/.pig/db.sqlite";
    const QString target_backup = "C:/PIG/.pig/db.sqlite~";
#else
    const QString target = QDir::homePath()+"/.pig/db.sqlite";
    const QString target_backup = QDir::homePath()+"/.pig/db.sqlite~";
#endif
        newDatabaseAvailable = false;
        QFile f(target);
        if (f.exists())
            f.rename(target_backup);
        if (QFile::copy(*path+*file, target)) {
            databaseUpdated = true;
            if (newBinaryAvailable) {
                get_files();
            } else {
                QFile f_backup(target_backup);
                f.remove();
                emit forwardSIGNAL();
            }
        } else {
            newBinaryAvailable = false;
            _root->setProperty("showNetwork", false);
            _root->setProperty("status", "");
            abort();
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

void Update::replace_binary_ready(int exitCode)
{
    _root->setProperty("showNetwork", false);
#ifdef _WIN32
    if (!databaseUpdated) {
        if (db->open()) {
            QSqlQuery qry;
            qry.prepare("UPDATE PigData SET BinaryVersion='"+QString::number(currentBinaryVersion)+"'");
            qry.prepare("UPDATE PigData SET Release='"+QString::number(currentRelease)+"'");
            qry.exec();
            db->close();
        }
    }
    const QString target_backup = "C:/PIG/.pig/db.sqlite~";
    QFile file_backup(target_backup);
    file_backup.remove();
    exit(0);
#else
    if (exitCode == 0) {
        if (!databaseUpdated) {
            if (db->open()) {
                QSqlQuery qry;
                qry.prepare("UPDATE PigData SET BinaryVersion='"+QString::number(currentBinaryVersion)+"'"); //TODO: No actualiza la db.
                qry.prepare("UPDATE PigData SET Release='"+QString::number(currentRelease)+"'");
                qry.exec();
                db->close();
            }
        }
        const QString target_backup = QDir::homePath()+"/.pig/db.sqlite~";
        QFile file_backup(target_backup);
        file_backup.remove();
        _root->setProperty("status", "UPDATED");
        _root->setProperty("information", "RESTART PIG TO APPLY CHANGES");
    } else {
        _root->setProperty("status", "UPDATE FAILED");
        _root->setProperty("information", "TRY LATER");
        QTimer::singleShot(3000, this, SLOT(abort())); 
    }
#endif
}

void Update::abort()
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
    const QString target_backup = "C:/PIG/.pig/db.sqlite~";
#else
    const QString target = QDir::homePath()+"/.pig/db.sqlite";
    const QString target_backup = QDir::homePath()+"/.pig/db.sqlite~";
#endif
    QFile file_backup(target_backup);
    if (databaseUpdated) {
        QFile file(target);
        file.remove();
        file_backup.rename(target);
    } else {
        if (file_backup.exists())
            file_backup.rename(target);
    }
    emit forwardSIGNAL();
}

