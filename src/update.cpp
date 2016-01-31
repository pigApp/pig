#include "update.h"
#include "threadedsocket.h"
#include "unpack.h"
#include "su.h"

#include <QProcess>
#include <QTextStream>
#include <QTimer>

const int RDM = (random() / (RAND_MAX + 1.0) * (((2 + 1) - 0) + 0));

Update::Update(const QString* const PIG_PATH, QSqlDatabase *db_, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _db(db_),
    hasNewBin(false),
    hasNewDb(false),
    hasNewLib(false),
    nUnpacked(0),
    ui(NULL)
{
    this->hide();

    if (_db->open()) {
        QSqlQuery query;

        query.prepare("SELECT binary, release, database, library, hostsUpdate, \
                       urlsUpdate, hostSite, urlSiteNews FROM data");

        if (!query.exec()) {
            _db->close();

            QTimer *t = new QTimer(this);

            QObject::connect (t, &QTimer::timeout, [&] { delete this; });

            t->setSingleShot(true);
            t->start(100);
        } else {
            query.next();

            QStringList _hosts = query.value(4).toString().split(",");
            QStringList _urls = query.value(5).toString().split(",");

            bin = query.value(0).toInt();
            rel = query.value(1).toInt();
            db = query.value(2).toInt();
            lib = query.value(3).toInt();
            host = _hosts[RDM];
            urls << _urls[RDM];
            hostSite = query.value(6).toString();
            urlSiteNews = query.value(7).toString();
            pkgs << NULL;

            _db->close();

            get();
        }
    } else {
        QTimer *t = new QTimer(this);

        QObject::connect (t, &QTimer::timeout, [&] { delete this; });

        t->setSingleShot(true);
        t->start(100);
    }
}

Update::~Update()
{
    if (ui != 0)
        delete ui;
}

void Update::get()
{
    ThreadedSocket *thread[pkgs.count()];

    for(int i = 0; i < pkgs.count(); i++) {
        thread[i] = new ThreadedSocket(_PIG_PATH, &host, &urls[i], &pkgs[i], i, this);

        connect (thread[i], SIGNAL(sendData(QString)), this, SLOT(check(QString)));
        connect (thread[i], SIGNAL(sendFile(int, QString)), this, SLOT(unpack(int, QString)));
        connect (thread[i], SIGNAL(sendError(QString)), this, SLOT(error(QString)));
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
        hasNewBin = true;
    }

    if (last[2].toInt() > db) {
        db = last[2].toInt();
        urls << last[6];
        sums << last[9];
        pkgs << "update_db.zip";
        hasNewDb = true;
    }

    if (last[3].toInt() > lib) {
        lib = last[3].toInt();
        urls << last[7];
        sums << last[10];
        pkgs << "update_lib.zip";
        hasNewLib = true;
    }

    if (hasNewBin || hasNewDb || hasNewLib) {
        init_ui();
    } else {
        delete this;
    }
}

void Update::unpack(int ID, QString path)
{
    Unpack *unpack = new Unpack(this);
    
    QObject::connect (unpack, &Unpack::sendExitCode, [&] (int exitCode) {
        unpack->deleteLater();

        if (exitCode == 0) {
            ++nUnpacked;
            if (pkgs.count() == nUnpacked)
                install();
            else
                ui->lb->setText("DOWNLOADING <html><img src=':/icon-download_status'></html>");
        } else {
            error("UNPACK FAILED");
        }
    });

    unpack->unzip(_PIG_PATH, &path, &sums[ID]);
}

void Update::install()
{
    ui->lb->setText("INSTALLING <html><img src=':/icon-install-dark'></html>");

    if (hasNewDb) {
        origin = *_PIG_PATH+"/tmp/update/db.sqlite";
        target = *_PIG_PATH+"/db.sqlite";
        backup = *_PIG_PATH+"/tmp/update/db.sqlite.bk";

        if (file.exists(target))
            file.rename(target, backup);
        if (file.rename(origin, target)) {
            if (!hasNewBin) {
                ui->lb->setText("DATABASE UPDATED");
                ui->b_1->setIcon(QIcon(":/icon-ok"));
                ui->b_1->setToolTip("ACCEPT");
                ui->b_2->setIcon(QIcon(":/icon-more"));
                ui->b_2->setToolTip("MORE INFO");
                ui->b_1->show();
                ui->b_2->show();
                QObject::connect (ui->b_1, &QPushButton::pressed, [&] { delete this; });
                QObject::connect (ui->b_2, &QPushButton::pressed, [&] {
                    QDesktopServices::openUrl(QUrl("http://"+hostSite+urlSiteNews));
                });
            }
        } else {
            error("UPDATE DATABASE FAILED");
        }
    }

    if (hasNewBin) {
#ifdef __linux__
    Su *su = new Su(this);

    QObject::connect (su, &Su::sendExitCode, [=] (int exitCode) {
        status(exitCode);
        su->deleteLater();
    });

    if (!hasNewLib) {
        su->install("'mv "+*_PIG_PATH+"/tmp/update/pig /usr/bin/ \
                    ; chown root.root /usr/bin/pig \
                    ; chmod +x /usr/bin/pig'");
    } else {
        su->install("'mv "+*_PIG_PATH+"/tmp/update/pig /usr/bin/ \
                    ; mv "+*_PIG_PATH+"/tmp/update/*.so* /usr/lib/pig/ \
                    ; chown root.root /usr/bin/pig \
                    ; chown root.root /usr/lib/pig/* \
                    ; chmod +x /usr/bin/pig \
                    ; chmod +x /usr/lib/pig/*'");
    }
#else
    QProcess proc; //FIX: USAR SEÑALES.

    if (!hasNewLib) {
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat"))
            status(0);
        else
            status(1);
    } else {
        if (proc.startDetached("C:\\PIG\\.pig\\update.bat lib"))
            status(0);
        else
            status(1);
    }
#endif
    }
}

void Update::status(const int &exitCode)
{
#ifdef __linux__
    if (exitCode == 0) {
        if (!hasNewDb) {
            if (_db->open()) {
                QSqlQuery query;

                query.prepare("UPDATE data SET binary='"+QString::number(bin)+"'");
                query.exec();
                query.prepare("UPDATE data SET release='"+QString::number(rel)+"'");
                query.exec();
                if (hasNewLib) {
                    query.prepare("UPDATE data SET library='"+QString::number(lib)+"'");
                    query.exec();
                }

                _db->close();
            }
        }

        ui->lb->setText("RESTART PIG");
        ui->b_1->setIcon(QIcon(":/icon-off"));
        ui->b_1->setToolTip("CLOSE PIG");
        ui->b_2->setIcon(QIcon(":/icon-more"));
        ui->b_2->setToolTip("MORE INFO");
        ui->b_1->show();
        ui->b_2->show();

        QObject::connect (ui->b_1, &QPushButton::pressed, [=] { exit(0); });
        QObject::connect (ui->b_2, &QPushButton::pressed, [&] {
            QDesktopServices::openUrl(QUrl("http://"+hostSite+urlSiteNews));
        });
    } else if (exitCode == -1) {
        error("UPDATE FAILED - REQUIRED GKSU/KDESU");
    } else {
        error("UPDATE FAILED");
    }
#else
    if (exitCode == 0) {
        if (!hasNewDb) {
            if (_db->open()) {
                QSqlQuery query;

                query.prepare("UPDATE data SET binary='"+QString::number(bin)+"'");
                query.exec();
                query.prepare("UPDATE data SET release='"+QString::number(rel)+"'");
                query.exec();
                if (hasNewLib) {
                    query.prepare("UPDATE data SET library='"+QString::number(lib)+"'");
                    query.exec();
                }

                _db->close();
            }
        }

        exit(0);
    } else {
        error("UPDATE FAILED");
    }
#endif
}

void Update::error(const QString &errorMsg)
{
    origin = *_PIG_PATH+"/db.sqlite";
    target = *_PIG_PATH+"/tmp/update/db.sqlite.trash";
    backup = *_PIG_PATH+"/tmp/update/db.sqlite.bk";

    if (hasNewDb && file.exists(backup)) {
        if (file.exists(origin)) {
            file.rename(origin, target);
            file.rename(backup, origin);
        } else {
            file.rename(backup, origin);
        }
    }
    
    if (ui != 0) {
        ui->lb->setText(errorMsg);
        ui->b_1->setIcon(QIcon(":/icon-error"));
        ui->b_1->setToolTip("CLOSE");
        ui->b_1->show();
        QObject::connect (ui->b_1, &QPushButton::pressed, [&] { delete this; });
    } else { 
        delete this;
    }
}

void Update::init_ui()
{
    ui = new Ui::Update;
    ui->setupUi(this);

    QObject::connect (ui->b_1, &QPushButton::pressed, [&] {
        ui->lb->setText("DOWNLOADING <html><img src=':/icon-download_status'></html>");
        ui->b_1->hide();
        ui->b_2->hide();
        ui->b_1->disconnect();
        ui->b_2->disconnect();

        get();
    });
    QObject::connect (ui->b_2, &QPushButton::pressed, [&] { delete this; });

    emit sendWidget(this);
}
