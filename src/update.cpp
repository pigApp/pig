#include "update.h"
#include "threadedsocket.h"
#include "unpack.h"

#include <QProcess>
#include <QTextStream>
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>//

Update::Update(const QString *PIG_PATH, QSqlDatabase *db, QObject *parent) : QObject(parent)
    ,_PIG_PATH(PIG_PATH)
    ,_db(db)
{
    su = NULL;
    group = NULL;

    hasBin = false;
    hasDb = false;
    hasLib = false;

    if (_db->open()) {
        QSqlQuery query;
        query.prepare("SELECT Binary, Release, Database, Library, Host, Url FROM PigData");
        if (!query.exec()) {
            _db->close();
            emit sig_error();
        } else {
            query.next();
            bin_v = query.value(0).toInt();
            rel_v = query.value(1).toInt();
            db_v = query.value(2).toInt();
            lib_v = query.value(3).toInt();
            host = query.value(4).toString();
            urls << query.value(5).toString();
            //urls << "/project/prepotest/bin/linux/x86_64/pig-bin-0.1.zip";//
            _db->close();

            pkgs << NULL;
            //pkgs << "update_bin.zip";

            get();
        }
    } else {
        emit sig_error();
    }
}

Update::~Update()
{
    qDebug() << "DELETE UPDATE";

    if (su != NULL)
        delete su;
}

void Update::get()
{
    ThreadedSocket *thread[pkgs.count()];

    for(int i = 0; i < pkgs.count(); i++) {
        thread[i] = new ThreadedSocket(_PIG_PATH, &host, &urls[i], &pkgs[i]);
        connect(thread[i], SIGNAL(sendData(QString)), this, SLOT(check(QString)));
        connect(thread[i], SIGNAL(sendFile(QString)), this, SLOT(unpack(QString)));
        connect(thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
        QObject::connect(thread[i], &ThreadedSocket::destroyed, [=] { qDebug() << "DELETE THREAD"; });//
        thread[i]->start();
    }
}

void Update::check(QString data)
{
    QStringList last_v;
    const QStringList split = data.split(QRegExp("[\r\n]"));
    const int i = sizeof(void*);

#ifdef __linux__
    if (i == 4)
        last_v = split[0].split(",");
    else
        last_v = split[1].split(",");
#else
    if (i == 4)
        last_v = split[2].split(",");
    else
        last_v = split[3].split(",");
#endif

    host = last_v[4];
    urls.clear();
    pkgs.clear();

    if ((last_v[0].toInt()+last_v[1].toInt()) > bin_v+rel_v) {
        new_bin_v = last_v[0].toInt();
        new_rel_v = last_v[1].toInt();
        urls << last_v[5];
        sums << last_v[8];
        pkgs << "update_bin.zip";
        hasBin = true;
    }
    if (last_v[2].toInt() > db_v) {
        new_db_v = last_v[2].toInt();
        urls << last_v[6];
        sums << last_v[9];
        pkgs << "update_db.zip";
        hasDb = true;
    }
    if (last_v[3].toInt() > lib_v) {
        new_lib_v = last_v[3].toInt();
        urls << last_v[7];
        sums << last_v[10];
        pkgs << "update_lib.zip";
        hasLib = true;
    }

    if (hasBin || hasDb || hasLib) {
        setup_ui();
    } else {
        emit finished();
    }
}

void Update::unpack(QString path)
{
    qDebug() << path;

    /*
    Unpack unpack;
    if (unpack.unpack(_PIG_PATH, path, &sums)) {
        update();
    } else {
        qDebug() << "UPDATE FAIL";//
        QTimer::singleShot(5000, this, SLOT(error()));
    }
    */
}

void Update::update()
{
    origin = *_PIG_PATH+"/tmp/news"; // TODO: SACAR news
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
                    new_bin_v = bin_v;
                    new_rel_v = rel_v;
                }
                if (!hasLib)
                    new_lib_v = lib_v;
                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(new_bin_v)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(new_rel_v)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(new_lib_v)+"'");
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
                    new_bin_v = bin_v;
                    new_rel_v = rel_v;
                }
                if (!hasLib)
                    new_lib_v = lib_v;
                QSqlQuery query;
                query.prepare("UPDATE PigData SET Binary='"+QString::number(new_bin_v)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(new_rel_v)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Library='"+QString::number(new_lib_v)+"'");
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

    QObject::connect(btnAccept, &QPushButton::clicked, [=] {
        label->setText("DOWNLOADING...");
        get();
    });

    QPushButton *btnCancel = new QPushButton("CANCEL", group);
    btnCancel->setFont(f);
    btnCancel->setPalette(p);
    btnCancel->setFlat(true);

    QObject::connect(btnCancel, &QPushButton::clicked, [&] { emit sendGroup(group); emit finished(); });

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(label);
    layout->addWidget(btnAccept);
    layout->addWidget(btnCancel);

    group->setLayout(layout);

    emit sendGroup(group, true);
}
