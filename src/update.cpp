#include "update.h"
#include "threadedsocket.h"
#include "unpack.h"
#include "su.h"

#include <QProcess>
#include <QTextStream>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>//

Update::Update(const QString *PIG_PATH, QSqlDatabase *db_, QObject *parent)
    : QObject(parent)
    , _PIG_PATH(PIG_PATH)
    , _db(db_)
{
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
        bin = last[0].toInt();
        rel = last[1].toInt();
        urls << last[5];
        sums << last[8];
        pkgs << "update_bin.zip";
        hasBin = true;
    }
    if (last[2].toInt() > db) {
        db = last[2].toInt();
        urls << last[6];
        sums << last[9];
        pkgs << "update_db.zip";
        hasDb = true;
    }
    if (last[3].toInt() > lib) {
        lib = last[3].toInt();
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
    qDebug() << "ID " << ID;//

    Unpack *unpack = new Unpack(this);

    QObject::connect (unpack, &Unpack::finished, [&] (int exitCode) {
        unpack->deleteLater();
        if (exitCode == 0) {
            ++nUnpacked;
            if (pkgs.count() == nUnpacked)
                update();
        } else {
            qDebug() << "ERROR UNPACK";//
            //error();
        }
    });

    unpack->unzip(_PIG_PATH, &path, &sums[ID]);
}

void Update::update()
{
//NEWS
    origin = *_PIG_PATH+"/tmp/news";
    target = *_PIG_PATH+"/news";

    file.rename(origin, target);
//NEWS

//DB
    if (hasDb) {
        origin = *_PIG_PATH+"/tmp/db.sqlite";
        target = *_PIG_PATH+"/db.sqlite";
        backup = *_PIG_PATH+"/tmp/db.sqlite.bk";

        if (file.exists(target))
            file.rename(target, backup);
        if (file.rename(origin, target)) {
            if (!hasBin) {
                emit sendGroup(group);
                emit finished();
            }
        } else {
            qDebug() << "ERROR UPDATE-DB";//
            //error();
        }
    }
//DB

//BIN-LIB
    if (hasBin) {
#ifdef __linux__
    Su *su = new Su(this);

    QObject::connect (su, &Su::finished, [=] (int exitCode) { status(exitCode); su->deleteLater(); });

    if (hasLib) {
        su->update("'mv "+*_PIG_PATH+"/tmp/pig /usr/bin/ \
                   ; mv "+*_PIG_PATH+"/tmp/*.so* /usr/lib/pig/ \
                   ; chown root.root /usr/bin/pig \
                   ; chown root.root /usr/lib/pig/* \
                   ; chmod +x /usr/bin/pig \
                   ; chmod +x /usr/lib/pig/*'");
    } else {
        su->update("'mv "+*_PIG_PATH+"/tmp/pig /usr/bin/ \
                   ; chown root.root /usr/bin/pig \
                   ; chmod +x /usr/bin/pig'");
    }
#else
    QProcess proc; //FIX: USAR SEÑALES.

    if (hasLib) {
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat lib"))
            status(0);
        else
            status(1);
    } else {
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat"))
            status(0);
        else
            status(1);
    }
#endif
    }
//BIN-LIB
}

void Update::status(int exitCode)
{
    qDebug() << "EXIT CODE " << exitCode;//

#ifdef __linux__
    if (exitCode == 0) {
        if (!hasDb) {
            if (_db->open()) {
                QSqlQuery query;

                query.prepare("UPDATE PigData SET Binary='"+QString::number(bin)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(rel)+"'");
                query.exec();
                if (hasLib) {
                    query.prepare("UPDATE PigData SET Library='"+QString::number(lib)+"'");
                    query.exec();
                }

                _db->close();
            }
        }
        qDebug() << "RESTART PIG";//
    } else if (exitCode == -1) {
        qDebug() << "ERROR SU";//
        //error();
    } else {
        qDebug() << "ERROR UPDATE-BIN/LIB";//
        //error();
    }
#else
    if (exitCode == 0) {
        if (!hasDb) {
            if (_db->open()) {
                QSqlQuery query;

                query.prepare("UPDATE PigData SET Binary='"+QString::number(bin)+"'");
                query.exec();
                query.prepare("UPDATE PigData SET Release='"+QString::number(rel)+"'");
                query.exec();
                if (hasLib) {
                    query.prepare("UPDATE PigData SET Library='"+QString::number(lib)+"'");
                    query.exec();
                }

                _db->close();
            }
        }
        exit(0);
    } else {
        qDebug() << "UPDATE FAIL";//
        //error();
    }
#endif
}

void Update::error()
{
//CLEAN UP NEWS
    origin = *_PIG_PATH+"/news";
    target = *_PIG_PATH+"/tmp/news.trash";

    if (file.exists(origin))
        file.rename(origin, target);
//CLEAN UP NEWS

//CLEAN UP DB
    origin = *_PIG_PATH+"/db.sqlite";
    target = *_PIG_PATH+"/tmp/db.sqlite.trash";
    backup = *_PIG_PATH+"/tmp/db.sqlite.bk";

    if (hasDb) {
        if (file.exists(origin) && file.exists(backup)) {
            file.rename(origin, target);
            file.rename(backup, origin);
        }
    }
//CLEAN UP DB

    //TODO: MOSTRAR MENSAJE Y BOTON DE ERROR.
    //emit finished();
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
    QBrush b1(QColor(30, 30, 30, 255));
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
