#include "update.h"
#include "unzip.h"

#include <QProcess>
#include <QTextStream>
#include <QTimer>

Update::Update(QObject *parent) : QObject(parent)
{
    mSu = NULL;
}

Update::~Update()
{
    mSocket->deleteLater();
    if (mSu != NULL)
        delete mSu;
    (*_root)->disconnect(this);
}

void Update::start()
{
    binaryAvailable = false;
    databaseAvailable = false;
    libraryAvailable = false;

    (*_root)->setProperty("showNetwork", true);

    QString Url;
    const int i = sizeof(void*);
#ifdef __linux__
    if (i == 4)
        Url = "UrlLinux_x86";
    else
        Url = "UrlLinux_x86_64";
#else
    if (i == 4)
        Url = "UrlWindows_32";
    else
        Url = "UrlWindows_64";
#endif
    if (db->open()) {
        QSqlQuery query;
        query.prepare("SELECT Binary, Release, Database, Library, Host, "+Url+" FROM PigData, UpdateData");
        if (!query.exec()) {
            db->close();
            emit signal_fail_database();
        } else {
            query.next();
            binary = query.value(0).toInt();
            release = query.value(1).toInt();
            database = query.value(2).toInt();
            library = query.value(3).toInt();
            const QString host = query.value(4).toString();
            QStringList urls; urls << query.value(5).toString();
            db->close();

            get(&host, &urls, "VERSIONS");
        }
    } else {
        emit signal_fail_database();
    }
}

void Update::get(const QString *const host, const QStringList *const urls, const QString request)
{
    if (request == "VERSIONS") {
        mSocket = new TcpSocket();
        connect (mSocket, SIGNAL(signal_ret_str(const QString *const)), this, SLOT(check_versions(const QString *const)));
        connect (mSocket, SIGNAL(signal_ret_files(const QString *const, const QStringList *const)), this,
                          SLOT(unzip_files(const QString *const, const QStringList *const)));
        connect (mSocket, SIGNAL(signal_fail_socket()), this, SLOT(error()));
    } else if (request == "UPDATE") {
        (*_root)->setProperty("status", "GETTING UPDATE");
    }
    mSocket->host = *host;
    mSocket->urls = *urls;
    mSocket->request = request;
    mSocket->start();
}

void Update::check_versions(const QString *const str)
{
    QStringList urls;
    const QStringList split = (*str).split(",");
    const QString host = split[4];
    if ((split[0].toInt()+split[1].toInt()) > binary+release) {
        newBinary = split[0].toInt();
        newRelease = split[1].toInt();
        urls << split[5];
        sums << split[8];
        binaryAvailable = true;
    }
    if (split[2].toInt() > database) {
        newDatabase = split[2].toInt();
        urls << split[6];
        sums << split[9];
        databaseAvailable = true;
    }
    if (split[3].toInt() > library) {
        newLibrary = split[3].toInt();
        urls << split[7];
        sums << split[10];
        libraryAvailable = true;
    }

    if (binaryAvailable || databaseAvailable || libraryAvailable) {
        get(&host, &urls, "UPDATE");
    } else {
        (*_root)->setProperty("showNetwork", false);
        emit signal_continue();
    }
}

void Update::unzip_files(const QString *const path, const QStringList *const files)
{
    (*_root)->setProperty("showNetwork", false);
    (*_root)->setProperty("status", "UPDATING...");

    Unzip mUnzip;
    if (mUnzip.unzip(&path, &files, &sums)) {
        update_files();
    } else {
        (*_root)->setProperty("status", "UPDATE FAILED");
        (*_root)->setProperty("information", "TRY LATER");
        QTimer::singleShot(3000, this, SLOT(error()));
    }
}

void Update::update_files()
{
    QFile file;
    QString origin;
    QString target;

//COPY NEWS
#ifdef __linux__
    origin = QDir::homePath()+"/.pig/tmp/news";
    target = QDir::homePath()+"/.pig/news";
#else
    origin = "C:/PIG/.pig/tmp/news";
    target = "C:/PIG/.pig/news";
#endif
    file.copy(origin, target);

//COPY DB
    if (databaseAvailable) {
#ifdef __linux__
    origin = QDir::homePath()+"/.pig/tmp/db.sqlite";
    target = QDir::homePath()+"/.pig/db.sqlite";
    const QString target_backup = QDir::homePath()+"/.pig/tmp/db.sqlite.bk";
#else
    origin = "C:/PIG/.pig/tmp/db.sqlite";
    target = "C:/PIG/.pig/db.sqlite";
    const QString target_backup = "C:/PIG/.pig/tmp/db.sqlite~";
#endif
        if (file.exists(target))
            file.rename(target, target_backup);
        if (file.copy(origin, target)) {
            if (!binaryAvailable && !libraryAvailable)
                emit signal_continue();
        } else {
            (*_root)->setProperty("status", "UPDATE FAILED");
            (*_root)->setProperty("information", "TRY LATER");
            QTimer::singleShot(3000, this, SLOT(error()));
        }
    }

//COPY BIN_LIB
    if (binaryAvailable) {
#ifdef __linux__
    mSu = new Su();
    connect (mSu, SIGNAL(signal_ret_su(int)), this, SLOT(check_exit(int)));
    if (!libraryAvailable)
        mSu->update("'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ ; chown root.root /usr/bin/pig ; chmod +x /usr/bin/pig'");
    else
        mSu->update("'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ ; mv "+QDir::homePath()+"/.pig/tmp/*.so* /usr/lib/pig/ ; \
                    chown root.root /usr/bin/pig ; chown root.root /usr/lib/pig/* ; chmod +x /usr/bin/pig ; chmod +x /usr/lib/pig/*'");
#else
    QProcess proc;
    if (!libraryAvailable)
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat"))
            check_exit(0);
        else
            check_exit(1);
    else
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat lib"))
            check_exit(0);
        else
            check_exit(1);
#endif
    }
}

void Update::check_exit(int exitCode)
{
#ifdef __linux__
    if (exitCode == 0) {
        if (!databaseAvailable) {
            if (db->open()) {
                if (!binaryAvailable) {
                    newBinary = binary;
                    newRelease = release;
                }
                if (!libraryAvailable)
                    newLibrary = library;
                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(newBinary)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(newRelease)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(newLibrary)+"'");
                query.exec();
                db->close();
            }
        }
        (*_root)->setProperty("status", "UPDATED");
        (*_root)->setProperty("information", "RESTART PIG");
    } else if (exitCode == -1) {
        (*_root)->setProperty("status", "UPDATE FAILED");
        (*_root)->setProperty("information", "Install gksu/kdesu");
        QTimer::singleShot(10000, this, SLOT(error()));
    } else {
        (*_root)->setProperty("status", "UPDATE FAILED");
        (*_root)->setProperty("information", "TRY LATER");
        QTimer::singleShot(3000, this, SLOT(error()));
    }
#else
    if (exitCode == 0) {
        if (!databaseAvailable) {
            if (db->open()) {
                if (!binaryAvailable) {
                    newBinary = binary;
                    newRelease = release;
                }
                if (!libraryAvailable)
                    newLibrary = library;
                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(newBinary)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(newRelease)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(newLibrary)+"'");
                query.exec();
                db->close();
            }
        }
        exit(0);
    } else {
        (*_root)->setProperty("status", "UPDATE FAILED");
        (*_root)->setProperty("information", "TRY LATER");
        QTimer::singleShot(3000, this, SLOT(error()));
    }
#endif
}

void Update::error()
{
    QFile file;
    QString target;

#ifdef __linux__
    target = QDir::homePath()+"/.pig/news";
    const QString tmp = QDir::homePath()+"/.pig/tmp/";
#else
    target = "C:/PIG/.pig/news";
    const QString tmp = "C:/PIG/.pig/tmp/";
#endif
    if (file.exists(target))
        file.rename(target, tmp+"news.trash");
    
#ifdef __linux__
    target = QDir::homePath()+"/.pig/db.sqlite";
    const QString target_backup = QDir::homePath()+"/.pig/tmp/db.sqlite.bk";
#else
    target = "C:/PIG/.pig/db.sqlite";
    const QString target_backup = "C:/PIG/.pig/tmp/db.sqlite.bk";
#endif
    if (databaseAvailable) {
        if (file.exists(target) && file.exists(target_backup))
            file.rename(target, tmp+"db.trash");
        file.rename(target_backup, target);
    }
    emit signal_continue();
}
