#include "update.h"
#include "unzip.h"

#include <QProcess>
#include <QTextStream>
#include <QTimer>
#include <QDebug>//

Update::Update(const QString *PIG_PATH, QSqlDatabase *db, QObject *parent)
    : QObject(parent)
    ,_PIG_PATH(PIG_PATH)
    ,_db(db)
{
    su = NULL;

    hasNewBin = false;
    hasNewDb = false;
    hasNewLib = false;

    //TODO: REMPLAZAR 'tmp' por 'PIG_PATH'.
}

Update::~Update()
{
    //delete socket;
    if (su != NULL)
        delete su;
}

void Update::start()
{
    if (_db->open()) {
        QSqlQuery query;
        query.prepare("SELECT Binary, Release, Database, Library, Host, Url FROM PigData");

        if (!query.exec()) {
            _db->close();
            emit sig_error();
        } else {
            query.next();

            bin = query.value(0).toInt();
            rel = query.value(1).toInt();
            db = query.value(2).toInt();
            lib = query.value(3).toInt();
            host = query.value(4).toString();
            urls << query.value(5).toString();

            _db->close();

            get();
        }
    } else {
        emit sig_error();
    }
}

void Update::get()
{
    socket = new TcpSocket(&_PIG_PATH, &host, &urls, &targets);

    connect (socket, SIGNAL(sendStr(const QString *const)) , this, SLOT(check_versions(const QString *const)));
    connect (socket, SIGNAL(sendFiles(const QStringList *const)) , this, SLOT(unzip_files(const QStringList *const)));
    connect (socket, SIGNAL(sig_err()), this, SLOT(error()));
}

void Update::check_versions(const QString *const str)
{
    QStringList last;
    const QStringList split = (*str).split(QRegExp("[\r\n]"));
    const int i = sizeof(void*);
#ifdef __linux__
    if (i == 4)
        last = split[0].split(",");
    else
        last = split[1].split(",");
#else
    if (i == 4)
        last = split[2].split(",");
    else
        last = split[3].split(",");
#endif
    urls.clear();
    host = last[4];

    if ((last[0].toInt()+last[1].toInt()) > bin+rel) {
        updatedBin = last[0].toInt();
        updatedRel = last[1].toInt();
        urls << last[5];
        sums << last[8];
        targets << "update_bin.zip";
        hasNewBin = true;
    }
    if (last[2].toInt() > db) {
        updatedDb = last[2].toInt();
        urls << last[6];
        sums << last[9];
        targets << "update_db.zip";
        hasNewDb = true;
    }
    if (last[3].toInt() > lib) {
        updatedLib = last[3].toInt();
        urls << last[7];
        sums << last[10];
        targets << "update_lib.zip";
        hasNewLib = true;
    }

    if (hasNewBin || hasNewDb || hasNewLib) {
        //(*_root)->setProperty("network", false);
        //(*_root)->setProperty("status", "UPDATE AVAILABLE");
        qDebug() << "UPDATE AVAILABLE";//
    } else {
        emit finished();
    }
}

void Update::user_confirmation()
{
    //(*_root)->setProperty("status", "");

    get();
}

void Update::unzip_files(const QStringList *const files)
{
    //(*_root)->setProperty("network", false);

    Unzip mUnzip;
    if (mUnzip.unzip(&_PIG_PATH, &files, &sums)) {
        update_files();
    } else {
        //(*_root)->setProperty("status", "FAIL");
        //(*_root)->setProperty("information", "TRY LATER");
        qDebug() << "UPDATE FAIL";//
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
    if (hasNewDb) {
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
            if (!hasNewBin && !hasNewLib)
                emit finished();
        } else {
            //(*_root)->setProperty("status", "FAIL");
            //(*_root)->setProperty("information", "TRY LATER");
            qDebug() << "UPDATE FAIL";//
            QTimer::singleShot(5000, this, SLOT(error()));
        }
    }

//COPY BIN/LIB
    if (hasNewBin) {
#ifdef __linux__
    su = new Su();
    connect (su, SIGNAL(sig_ret_su(int)), this, SLOT(check_exit(int)));
    if (!hasNewLib)
        su->update("'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ \
            ; chown root.root /usr/bin/pig ; chmod +x /usr/bin/pig'");
    else
        su->update("'mv "+QDir::homePath()+"/.pig/tmp/pig /usr/bin/ ; mv "
            +QDir::homePath()+"/.pig/tmp/*.so* /usr/lib/pig/ ; \
            chown root.root /usr/bin/pig ; chown root.root /usr/lib/pig/* \
            ; chmod +x /usr/bin/pig ; chmod +x /usr/lib/pig/*'");
#else
    QProcess proc;
    if (!hasNewLib)
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
        if (!hasNewDb) {
            if (_db->open()) {
                if (!hasNewBin) {
                    updatedBin = bin;
                    updatedRel = rel;
                }
                if (!hasNewLib)
                    updatedLib = lib;
                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(updatedBin)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(updatedRel)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(updatedLib)+"'");
                query.exec();
                _db->close();
            }
        }
        //(*_root)->setProperty("status", "DONE");
        //(*_root)->setProperty("information", "RESTART PIG");
        qDebug() << "RESTART PIG";
    } else if (exitCode == -1) {
        //(*_root)->setProperty("status", "FAIL");
        //(*_root)->setProperty("information", "GKSU·KDESU NEEDED");
        qDebug() << "GKSU-KDESU NEEDED";
        QTimer::singleShot(10000, this, SLOT(error()));
    } else {
        //(*_root)->setProperty("status", "FAIL");
        //(*_root)->setProperty("information", "TRY LATER");
        qDebug() << "UPDATE FAIL";
        QTimer::singleShot(5000, this, SLOT(error()));
    }
#else
    if (exitCode == 0) {
        if (!hasNewDb) {
            if (_db->open()) {
                if (!hasNewBin) {
                    updatedBin = bin;
                    updatedRel = rel;
                }
                if (!hasNewLib)
                    updatedLib = lib;
                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(updatedBin)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(updatedRel)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(updatedLib)+"'");
                query.exec();
                _db->close();
            }
        }
        exit(0);
    } else {
        //(*_root)->setProperty("status", "FAIL");
        //(*_root)->setProperty("information", "TRY LATER");
        qDebug() << "UPDATE FAIL";
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
    if (hasNewDb) {
        if (file.exists(target) && file.exists(target_backup))
            file.rename(target, tmp+"db.trash");
        file.rename(target_backup, target);
    }
    emit finished();
}
// Tabs hechos.
