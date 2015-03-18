#include "update.h"
#include "unzip.h"

#include <QProcess>
#include <QTextStream>
#include <QTimer>

Update::Update(QObject *parent, QObject **root, QSqlDatabase *db)
    : QObject(parent)
{
    mSu = NULL;
    _root = root;
    _db = db;
    binaryAvailable = false;
    databaseAvailable = false;
    libraryAvailable = false;

    start();
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
    if (_db->open()) {
        QSqlQuery query;
        query.prepare("SELECT Binary, Release, Database, Library, Host, "
            +Url+" FROM PigData, UpdateData");
        if (!query.exec()) {
            _db->close();
            emit sig_fail_database();
        } else {
            query.next();
            binary = query.value(0).toInt();
            release = query.value(1).toInt();
            database = query.value(2).toInt();
            library = query.value(3).toInt();
            host = query.value(4).toString();
            urls << query.value(5).toString();
            _db->close();

            get(&host, &urls, &targets);
        }
    } else {
        emit sig_fail_database();
    }
}

void Update::get(const QString *const host ,const QStringList *const urls
    ,const QStringList *const targets)
{
    if (targets->isEmpty()) {
        mSocket = new TcpSocket();
        connect (mSocket, SIGNAL(sig_ret_string(const QString *const))
            , this, SLOT(check_versions(const QString *const)));
        connect (mSocket, SIGNAL(sig_ret_files(const QString *const, const QStringList *const))
            , this, SLOT(unzip_files(const QString *const, const QStringList *const)));
        connect (mSocket, SIGNAL(sig_socket_err()), this, SLOT(error()));
    }
    mSocket->host = *host;
    mSocket->urls = *urls;
    mSocket->targets = *targets;
    mSocket->start();

    (*_root)->setProperty("network", true);
}

void Update::check_versions(const QString *const str)
{
    const QStringList split = (*str).split(",");
    urls.clear();
    host = split[4];
    if ((split[0].toInt()+split[1].toInt()) > binary+release) {
        newBinary = split[0].toInt();
        newRelease = split[1].toInt();
        urls << split[5];
        sums << split[8];
        targets << "update_bin.zip";
        binaryAvailable = true;
    }
    if (split[2].toInt() > database) {
        newDatabase = split[2].toInt();
        urls << split[6];
        sums << split[9];
        targets << "update_db.zip";
        databaseAvailable = true;
    }
    if (split[3].toInt() > library) {
        newLibrary = split[3].toInt();
        urls << split[7];
        sums << split[10];
        targets << "update_lib.zip";
        libraryAvailable = true;
    }

    if (binaryAvailable || databaseAvailable || libraryAvailable) {
        (*_root)->setProperty("network", false);
        (*_root)->setProperty("status", "UPDATE AVAILABLE");
    } else {
        emit sig_continue();
    }
}

void Update::user_confirmation()
{
    (*_root)->setProperty("status", "");

    get(&host, &urls, &targets);
}

void Update::unzip_files(const QString *const tmp, const QStringList *const files)
{
    (*_root)->setProperty("network", false);

    Unzip mUnzip;
    if (mUnzip.unzip(&tmp, &files, &sums)) {
        update_files();
    } else {
        (*_root)->setProperty("status", "FAIL");
        (*_root)->setProperty("information", "TRY LATER");
        QTimer::singleShot(5000, this, SLOT(error()));
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
                emit sig_continue();
        } else {
            (*_root)->setProperty("status", "FAIL");
            (*_root)->setProperty("information", "TRY LATER");
            QTimer::singleShot(5000, this, SLOT(error()));
        }
    }

//COPY BIN/LIB
    if (binaryAvailable) {
#ifdef __linux__
    mSu = new Su();
    connect (mSu, SIGNAL(sig_ret_su(int)), this, SLOT(check_exit(int)));
    if (!libraryAvailable)
        mSu->update("'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ \
            ; chown root.root /usr/bin/pig ; chmod +x /usr/bin/pig'");
    else
        mSu->update("'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ ; mv "
            +QDir::homePath()+"/.pig/tmp/*.so* /usr/lib/pig/ ; \
            chown root.root /usr/bin/pig ; chown root.root /usr/lib/pig/* \
            ; chmod +x /usr/bin/pig ; chmod +x /usr/lib/pig/*'");
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
            if (_db->open()) {
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
                _db->close();
            }
        }
        (*_root)->setProperty("status", "UPDATED");
        (*_root)->setProperty("information", "RESTART PIG");
    } else if (exitCode == -1) {
        (*_root)->setProperty("status", "FAIL");
        (*_root)->setProperty("information", "GKSU·KDESU NEEDED");
        QTimer::singleShot(10000, this, SLOT(error()));
    } else {
        (*_root)->setProperty("status", "FAIL");
        (*_root)->setProperty("information", "TRY LATER");
        QTimer::singleShot(5000, this, SLOT(error()));
    }
#else
    if (exitCode == 0) {
        if (!databaseAvailable) {
            if (_db->open()) {
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
                _db->close();
            }
        }
        exit(0);
    } else {
        (*_root)->setProperty("status", "FAIL");
        (*_root)->setProperty("information", "TRY LATER");
        QTimer::singleShot(5000, this, SLOT(error()));
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
    emit sig_continue();
}
// Tabs hechos.
