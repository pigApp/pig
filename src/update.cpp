#include "update.h"
#include "threadedsocket.h"
#include "unpack.h"
#include "su.h"

#include <QProcess>
#include <QTextStream>
#include <QTimer>

Update::Update(const QString *PIG_PATH, QSqlDatabase *db_, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _db(db_),
    ui(NULL)
{
    hasBin = false;
    hasDb = false;
    hasLib = false;
    nUnpacked = 0;

    this->hide();

    if (_db->open()) {
        QSqlQuery query;
        query.prepare("SELECT binary, release, database, library, hostUpdate \
                      , urlUpdate, hostSite, urlSiteNews FROM data");

        if (!query.exec()) {
            _db->close();
            QTimer::singleShot(100, this, SLOT(error()));
        } else {
            query.next();

            bin = query.value(0).toInt();
            rel = query.value(1).toInt();
            db = query.value(2).toInt();
            lib = query.value(3).toInt();
            host = query.value(4).toString();
            urls << query.value(5).toString();
            hostSite = query.value(6).toString();
            urlSiteNews = query.value(7).toString();
            pkgs << NULL;

            _db->close();

            get();
        }
    } else {
        QTimer::singleShot(100, this, SLOT(error()));
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
        init_ui();
    } else {
        delete this;
    }
}

void Update::unpack(int ID, QString path)
{
    ui->label->setText("UNPACKING");

    Unpack *unpack = new Unpack(this);

    QObject::connect (unpack, &Unpack::finished, [&] (int exitCode) {
        unpack->deleteLater();
        if (exitCode == 0) {
            ++nUnpacked;
            if (pkgs.count() == nUnpacked)
                install();
            else
                ui->label->setText("DOWNLOADING");
        } else {
            error("UNPACK FAILED");
        }
    });

    unpack->unzip(_PIG_PATH, &path, &sums[ID]);
}

void Update::install()
{
    ui->label->setText("INSTALLING");

    if (hasDb) {
        origin = *_PIG_PATH+"/tmp/update/db.sqlite";
        target = *_PIG_PATH+"/db.sqlite";
        backup = *_PIG_PATH+"/tmp/update/db.sqlite.bk";

        if (file.exists(target))
            file.rename(target, backup);
        if (file.rename(origin, target)) {
            if (!hasBin) {
                ui->label->setText("DATABASE UPDATED");
                ui->button_a->setText("OK");
                ui->button_b->setText("INFO");
                ui->button_a->show();
                ui->button_b->show();
                QObject::connect (ui->button_a, &QPushButton::pressed, [&] { delete this; });
                QObject::connect (ui->button_b, &QPushButton::pressed, [&] {
                    QDesktopServices::openUrl(QUrl("http://"+hostSite+urlSiteNews));
                });
            }
        } else {
            error("UPDATE DATABASE FAILED");
        }
    }

    if (hasBin) {
#ifdef __linux__
    Su *su = new Su(this);

    QObject::connect (su, &Su::finished, [=] (int exitCode) {
        status(exitCode);
        su->deleteLater();
    });

    if (!hasLib) {
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

    if (!hasLib) {
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
        if (!hasDb) {
            if (_db->open()) {
                QSqlQuery query;

                query.prepare("UPDATE data SET binary='"+QString::number(bin)+"'");
                query.exec();
                query.prepare("UPDATE data SET release='"+QString::number(rel)+"'");
                query.exec();
                if (hasLib) {
                    query.prepare("UPDATE data SET library='"+QString::number(lib)+"'");
                    query.exec();
                }

                _db->close();
            }
        }
        ui->label->setText("UPDATED. RESTART PIG");
        ui->button_a->setText("EXIT");
        ui->button_b->setText("INFO");
        ui->button_a->show();
        ui->button_b->show();
        QObject::connect (ui->button_a, &QPushButton::pressed, [=] { exit(0); });
        QObject::connect (ui->button_b, &QPushButton::pressed, [&] {
            QDesktopServices::openUrl(QUrl("http://"+hostSite+urlSiteNews));
        });
    } else if (exitCode == -1) {
        error("UPDATE FAILED. REQUIRED GKSU/KDESU");
    } else {
        error("UPDATE FAILED");
    }
#else
    if (exitCode == 0) {
        if (!hasDb) {
            if (_db->open()) {
                QSqlQuery query;

                query.prepare("UPDATE data SET binary='"+QString::number(bin)+"'");
                query.exec();
                query.prepare("UPDATE data SET release='"+QString::number(rel)+"'");
                query.exec();
                if (hasLib) {
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

void Update::error(const QString &error)
{
    origin = *_PIG_PATH+"/db.sqlite";
    target = *_PIG_PATH+"/tmp/update/db.sqlite.trash";
    backup = *_PIG_PATH+"/tmp/update/db.sqlite.bk";

    if (hasDb && file.exists(backup)) {
        if (file.exists(origin)) {
            file.rename(origin, target);
            file.rename(backup, origin);
        } else {
            file.rename(backup, origin);
        }
    }

    if (ui != 0) {
        ui->label->setText(error);
        ui->button_a->setPalette(ui->palette_error);
        ui->button_a->setText("OK");
        ui->button_a->show();
        QObject::connect (ui->button_a, &QPushButton::pressed, [&] { delete this; });
    } else {
        emit dbError("DATABASE CORRUPTED");
        delete this;
    }
}

void Update::init_ui()
{
    ui = new Ui::Update;
    ui->setupUi(this);

    QObject::connect (ui->button_a, &QPushButton::pressed, [&] {
        ui->label->setText("DOWNLOADING");
        ui->button_a->hide();
        ui->button_b->hide();
        ui->button_a->disconnect();
        ui->button_b->disconnect();
        get();
    });
    QObject::connect (ui->button_b, &QPushButton::pressed, [&] { delete this; });

    emit showWidget(this);
}
