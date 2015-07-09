#include "update.h"
#include "threadedsocket.h"
#include "unpack.h"

#include <QProcess>
#include <QTextStream>
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>//

Update::Update(const QString *PIG_PATH, QSqlDatabase *db_, QObject *parent)
    : QObject(parent)
    , _PIG_PATH(PIG_PATH)
    , _db(db_)
{
    su = NULL;
    group = NULL;

    hasBin = false;
    hasDb = false;
    hasLib = false;

    nUnpacked = 0;

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
            pkgs << NULL;

            _db->close();

            get();
        }
    } else {
        emit sig_error();
    }
}

Update::~Update()
{
    if (su != NULL)
        delete su;
}

void Update::get()
{
    ThreadedSocket *thread[pkgs.count()];

    for(int i = 0; i < pkgs.count(); i++) {
        thread[i] = new ThreadedSocket(_PIG_PATH, &host, &urls[i], &pkgs[i], i, this);
        connect (thread[i], SIGNAL(sendData(QString)), this, SLOT(check(QString)));
        connect (thread[i], SIGNAL(sendFile(QString, int)), this, SLOT(unpack(QString, int)));
        connect (thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
        thread[i]->start();
    }
}

void Update::check(QString data)
{
    const QStringList dataSplit = data.split(QRegExp("[\r\n]"));
    QStringList last;
    const int arch = (sizeof(void*))*8;

#ifdef __linux__
    if (arch == 32)
        last = dataSplit[0].split(",");
    else
        last = dataSplit[1].split(",");
#else
    if (arch == 32)
        last = dataSplit[2].split(",");
    else
        last = dataSplit[3].split(",");
#endif

    host = last[4];
    urls.clear();
    pkgs.clear();

    if ((last[0].toInt()+last[1].toInt()) > bin+rel) {
        newBin = last[0].toInt();
        newRel = last[1].toInt();
        urls << last[5];
        sums << last[8];
        pkgs << "update_bin.zip";
        hasBin = true;
    }
    if (last[2].toInt() > db) {
        newDb = last[2].toInt();
        urls << last[6];
        sums << last[9];
        pkgs << "update_db.zip";
        hasDb = true;
    }
    if (last[3].toInt() > lib) {
        newLib = last[3].toInt();
        urls << last[7];
        sums << last[10];
        pkgs << "update_lib.zip";
        hasLib = true;
    }

    if (hasBin || hasDb || hasLib) {
        setup_ui();
    } else {
        emit finished();
    }
}

void Update::unpack(QString path, int ID)
{
    Unpack *unpack = new Unpack(this);

    QObject::connect (unpack, &Unpack::finished, [&] (int exitCode) {
        unpack->deleteLater();
        if (exitCode == 0) {
            ++nUnpacked;
            if (pkgs.count() == nUnpacked)
                update();
        } else {
            error();
        }
    });

    unpack->unzip(_PIG_PATH, &path, &sums[ID]);
}

void Update::update()
{
    origin = *_PIG_PATH+"/tmp/news";
    target = *_PIG_PATH+"/news";

    file.copy(origin, target);

    if (hasDb) {
        origin = *_PIG_PATH+"/tmp/db.sqlite";
        target = *_PIG_PATH+"/db.sqlite";
        backup = *_PIG_PATH+"/tmp/db.sqlite.bk";

        if (file.exists(target))
            file.rename(target, backup);
        if (file.copy(origin, target)) {
            if (!hasBin && !hasLib)
                emit finished();
        } else {
            qDebug() << "UPDATE FAIL";//
            QTimer::singleShot(5000, this, SLOT(error()));
        }
    }

    if (hasBin) {
#ifdef __linux__
    su = new Su();

    connect (su, SIGNAL(sig_ret_su(int)), this, SLOT(status(int)));

    if (!hasLib)
        su->update("'mv "+*_PIG_PATH+"/tmp/pig /usr/bin/ \
                   ; chown root.root /usr/bin/pig \
                   ; chmod +x /usr/bin/pig'");
    else
        su->update("'mv "+*_PIG_PATH+"/tmp/pig /usr/bin/ \
                   ; mv "+*_PIG_PATH+"/tmp/*.so* /usr/lib/pig/ \
                   ; chown root.root /usr/bin/pig \
                   ; chown root.root /usr/lib/pig/* \
                   ; chmod +x /usr/bin/pig \
                   ; chmod +x /usr/lib/pig/*'");
#else
    QProcess proc;
    if (!hasLib)
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat"))
            status(0);
        else
            status(1);
    else
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat lib"))
            status(0);
        else
            status(1);
#endif
    }
}

void Update::status(int exitCode)
{
#ifdef __linux__
    if (exitCode == 0) {
        if (!hasDb) {
            if (_db->open()) {
                if (!hasBin) {
                    newBin = bin;//FIX: NO USAR 'newBin, newRel...'
                    newRel = rel;
                }
                if (!hasLib)
                    newLib = lib;

                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(newBin)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(newRel)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(newLib)+"'");

                query.exec();

                _db->close();
            }
        }
        qDebug() << "RESTART PIG";
    } else if (exitCode == -1) {
        qDebug() << "GKSU-KDESU NEEDED";
        QTimer::singleShot(10000, this, SLOT(error()));
    } else {
        qDebug() << "UPDATE FAIL";
        QTimer::singleShot(5000, this, SLOT(error()));
    }
#else
    if (exitCode == 0) {
        if (!hasDb) {
            if (_db->open()) {
                if (!hasBin) {
                    newBin = bin;
                    newRel = rel;
                }
                if (!hasLib)
                    newLib = lib;
                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(newBin)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(newRel)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(newLib)+"'");
                query.exec();
                _db->close();
            }
        }
        exit(0);
    } else {
        qDebug() << "UPDATE FAIL";
        QTimer::singleShot(5000, this, SLOT(error()));
    }
#endif
}

void Update::error()
{
    origin = *_PIG_PATH+"/news";
    target = *_PIG_PATH+"/tmp/news.trash";

    if (file.exists(origin))
        file.rename(origin, target);

    origin = *_PIG_PATH+"/db.sqlite";
    target = *_PIG_PATH+"/tmp/db.sqlite.trash";
    backup = *_PIG_PATH+"/tmp/db.sqlite.bk";

    if (hasDb) {
        if (file.exists(origin) && file.exists(backup))
            file.rename(origin, target);
        file.rename(backup, origin);
    }

    emit finished();
}

void Update::setup_ui()
{
    group = new QGroupBox;
    group->setStyleSheet("QGroupBox{ border:0; }");
    group->setFlat(true);

    QFont f(":/font-global");
    f.setPointSize(24); //TODO: CAMBIAR A PORCENTAJE
    f.setBold(true);

    QBrush b(QColor(0, 0, 0, 255));
    QBrush b1(QColor(63, 63, 63, 255));
    QBrush b2(QColor(255, 255, 255, 255));

    QPalette p;
    p.setBrush(QPalette::Active, QPalette::Button, b);
    p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
    p.setBrush(QPalette::Active, QPalette::Text, b2);
    p.setBrush(QPalette::Active, QPalette::Base, b);
    p.setBrush(QPalette::Active, QPalette::Window, b);
    p.setBrush(QPalette::Active, QPalette::WindowText, b2);
    p.setBrush(QPalette::Active, QPalette::Highlight, b);
    p.setBrush(QPalette::Disabled, QPalette::Button, b);
    p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
    p.setBrush(QPalette::Disabled, QPalette::Text, b1);
    p.setBrush(QPalette::Disabled, QPalette::Base, b);
    p.setBrush(QPalette::Disabled, QPalette::Window, b);
    p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
    p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

    label = new QLabel(group);
    label->setFont(f);
    label->setPalette(p);
    label->setText("UPDATE AVAILABLE");

    QPushButton *btnAccept = new QPushButton("ACCEPT", group);
    btnAccept->setFont(f);
    btnAccept->setPalette(p);
    btnAccept->setFlat(true);

    QObject::connect (btnAccept, &QPushButton::clicked, [=] {
        label->setText("DOWNLOADING...");
        get();
    });

    QPushButton *btnCancel = new QPushButton("CANCEL", group);
    btnCancel->setFont(f);
    btnCancel->setPalette(p);
    btnCancel->setFlat(true);

    QObject::connect (btnCancel, &QPushButton::clicked, [&] { emit sendGroup(group); emit finished(); });

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(label);
    layout->addWidget(btnAccept);
    layout->addWidget(btnCancel);

    group->setLayout(layout);

    emit sendGroup(group, true);
}
