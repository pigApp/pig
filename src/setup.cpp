#include "setup.h"
#include "authorization.h"

#include <QTextStream>
#include <QTimer>

Setup::Setup(const QString* const PIG_PATH, bool *keep_covers, bool *keep_torrents,
             bool *keep_movies, int *torrent_port_1, int *torrent_port_2,
             QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _db(db),
    ui(new Ui::Setup)
{
    QStringList data;

    if (_db->open()) {
        QSqlQuery query;

        query.prepare("SELECT binary, release, database, hostSite, urlSiteTorrents \
                      , urlSiteCode, urlSiteBugs, urlSiteHelp FROM data");

        if (!query.exec()) {
            _db->close();

            QTimer *t = new QTimer(this);

            QObject::connect (t, &QTimer::timeout, [&] { emit sendError("DATABASE CORRUPTED"); });

            t->setSingleShot(true);
            t->start(0);
        } else {
            query.next();

            data.append(query.value(0).toString());
            data.append(query.value(1).toString());
            data.append(query.value(2).toString());
            data.append(query.value(3).toString());
            data.append(query.value(4).toString());
            data.append(query.value(5).toString());
            data.append(query.value(6).toString());
            data.append(query.value(7).toString());
            data.append(query.value(8).toString());

            _db->close();
        }
    } else {
        QTimer *t = new QTimer(this);

        QObject::connect (t, &QTimer::timeout, [&] { emit sendError("DATABASE CORRUPTED"); });

        t->setSingleShot(true);
        t->start(0);
    }

    if (!data.isEmpty()) {
        Authorization *authorization = new Authorization(PIG_PATH, true, this);
        authorization->check();

        ui->setupUi(&data, &keep_covers, &keep_torrents, &keep_movies,
                    &torrent_port_1, &torrent_port_2, authorization, this);

        QObject::connect (ui->r_folder_covers, &QPushButton::pressed, [=] {
            *keep_covers = !*keep_covers;
            set_rc("KEEP_LOCAL_COPY_OF_COVERS", *keep_covers);
        });
        QObject::connect (ui->r_folder_torrents, &QPushButton::pressed, [=] {
            *keep_torrents = !*keep_torrents;
            set_rc("KEEP_LOCAL_COPY_OF_TORRENTS", *keep_torrents);
        });
        QObject::connect (ui->r_folder_movies, &QPushButton::pressed, [=] {
            *keep_movies = !*keep_movies;
            set_rc("KEEP_LOCAL_COPY_OF_MOVIES", *keep_movies);
        });

        QObject::connect (ui->b_folder_covers_reset, &QPushButton::pressed, [&] {
            if (clean_folder("covers") && clean_folder("covers/back")) {
                set_icon(&ui->b_folder_covers_reset);
                emit folderCoversReset();
            } else {
                set_icon(&ui->b_folder_covers_reset, false, true);
            }
        });
        QObject::connect (ui->b_folder_torrents_reset, &QPushButton::pressed, [&] {
            if (clean_folder("torrents"))
                set_icon(&ui->b_folder_torrents_reset);
            else
                set_icon(&ui->b_folder_torrents_reset, false, true);
        });
        QObject::connect (ui->b_folder_movies_reset, &QPushButton::pressed, [&] {
            if (clean_folder("torrents/movies"))
                set_icon(&ui->b_folder_movies_reset);
            else
                set_icon(&ui->b_folder_movies_reset, false, true);
        });

        QObject::connect (ui->input_torrent_port_1, &QLineEdit::cursorPositionChanged, [&] {
            ui->input_torrent_port_1->setPalette(ui->p_torrent_edit);

            if ((ui->input_torrent_port_1->text().toInt() == 6900)
                && (ui->input_torrent_port_2->text().toInt() == 6999)) {
                ui->b_torrent_ports_reset->setIcon(QIcon(":/icon-reset-dark"));
                ui->b_torrent_ports_reset->setDisabled(true);
            } else {
                ui->b_torrent_ports_reset->setIcon(QIcon(":/icon-reset"));
                ui->b_torrent_ports_reset->setEnabled(true);
            }
        });
        QObject::connect (ui->input_torrent_port_1, &QLineEdit::editingFinished, [=] {
            if (set_rc("TORRENT_PORT_1", *torrent_port_1)) {
                *torrent_port_1 = ui->input_torrent_port_1->text().toInt();

                if ((ui->input_torrent_port_1->text().toInt() == 6900)
                    && (ui->input_torrent_port_2->text().toInt() == 6999))
                    set_icon(&ui->b_torrent_ports_reset);
                else
                    set_icon(&ui->b_torrent_ports_reset, false);
            } else {
                ui->input_torrent_port_1->setText(QString::number(*torrent_port_1));
                set_icon(&ui->b_torrent_ports_reset, false, true);
            }

            ui->input_torrent_port_1->setPalette(ui->p_torrent);
        });

        QObject::connect (ui->input_torrent_port_2, &QLineEdit::cursorPositionChanged, [&] {
            ui->input_torrent_port_2->setPalette(ui->p_torrent_edit);

            if ((ui->input_torrent_port_1->text().toInt() == 6900)
                && (ui->input_torrent_port_2->text().toInt() == 6999)) {
                ui->b_torrent_ports_reset->setIcon(QIcon(":/icon-reset-dark"));
                ui->b_torrent_ports_reset->setDisabled(true);
            } else {
                ui->b_torrent_ports_reset->setIcon(QIcon(":/icon-reset"));
                ui->b_torrent_ports_reset->setEnabled(true);
            }
        });
        QObject::connect (ui->input_torrent_port_2, &QLineEdit::editingFinished, [=] {
            if (set_rc("TORRENT_PORT_2", *torrent_port_2)) {
                *torrent_port_2 = ui->input_torrent_port_2->text().toInt();

                if ((ui->input_torrent_port_1->text().toInt() == 6900)
                    && (ui->input_torrent_port_2->text().toInt() == 6999))
                    set_icon(&ui->b_torrent_ports_reset);
                else
                    set_icon(&ui->b_torrent_ports_reset, false);
            } else {
                ui->input_torrent_port_2->setText(QString::number(*torrent_port_2));

                set_icon(&ui->b_torrent_ports_reset, false, true);
            }

            ui->input_torrent_port_2->setPalette(ui->p_torrent);
        });

        QObject::connect (ui->b_torrent_ports_reset, &QPushButton::pressed, [=] {
            if (set_rc("TORRENT_PORT_1", 6900)
                && set_rc("TORRENT_PORT_2", 6999)) {
                *torrent_port_1 = 6900;
                *torrent_port_2 = 6999;

                ui->input_torrent_port_1->setText("6900");
                ui->input_torrent_port_2->setText("6999");
                ui->input_torrent_port_1->setPalette(ui->p_torrent);
                ui->input_torrent_port_2->setPalette(ui->p_torrent);

                set_icon(&ui->b_torrent_ports_reset);
            } else {
                set_icon(&ui->b_torrent_ports_reset, false, true);
            }
        });

        QObject::connect (ui->b_contribute_torrent, &QPushButton::pressed, [=] {
            QDesktopServices::openUrl(QUrl("http://"+data[3]+data[4]));
        });
        QObject::connect (ui->b_contribute_code, &QPushButton::pressed, [=] {
            QDesktopServices::openUrl(QUrl("http://"+data[3]+data[5]));
        });
        QObject::connect (ui->b_contribute_bug, &QPushButton::pressed, [=] {
            QDesktopServices::openUrl(QUrl("http://"+data[3]+data[6]));
        });
        QObject::connect (ui->b_contribute_support, &QPushButton::pressed, [&] {
            if (ui->lb_contribute_support->text() == "SUPPORT") {
                ui->lb_contribute_support->setText("BITCOIN");
                ui->b_contribute_support->setIcon(QIcon(":/icon-less"));
                ui->lb_contribute_wallet->setPalette(ui->p_wallet);
            } else {
                ui->lb_contribute_support->setText("SUPPORT");
                ui->b_contribute_support->setIcon(QIcon(":/icon-more"));
            }

            ui->lb_contribute_wallet->setHidden(!ui->lb_contribute_wallet->isHidden());
            ui->b_contribute_copy_wallet->setHidden(!ui->b_contribute_copy_wallet->isHidden());
        });
        QObject::connect (ui->b_contribute_copy_wallet, &QPushButton::pressed, [&] {
            ui->clipboard->clear();
            ui->clipboard->setText(ui->lb_contribute_wallet->text());
            ui->lb_contribute_wallet->setPalette(ui->p_wallet_copied);
        });
    }
}

Setup::~Setup()
{
    delete ui;
}

bool Setup::set_rc(const QString &option, const QVariant &value)
{
    QFile file(*_PIG_PATH+"/.pigrc");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&file);
        QString out;

        while (!stream.atEnd()) {
            QString line = stream.readLine().simplified();
            if (line.section("=", 0, 0) == option)
                line.replace(line, option+"="+value.toString());
            out.append(line+"\n");
        }

        file.resize(0);
        stream << out;
        file.close();

        return true;
    }

    return false;
}

bool Setup::clean_folder(const QString &folder)
{
    QDir dir(*_PIG_PATH+"/tmp/"+folder);
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    if (dir.exists()) {
        foreach (QString file, dir.entryList())
            dir.remove(file);

        return true;
    }

    return false;
}

void Setup::set_icon(QPushButton **button, const bool &setDisabled, const bool &hasFailed)
{
    if (hasFailed)
        (*button)->setIcon(QIcon(":/icon-cancel"));
    else
        (*button)->setIcon(QIcon(":/icon-ok"));

    (*button)->setEnabled(true);

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->start(1000);

    if (hasFailed) {
        QObject::connect(timer, &QTimer::timeout, [=] {
            (*button)->setIcon(QIcon(":/icon-reset"));
        });
    } else {
        QObject::connect(timer, &QTimer::timeout, [=] {
            if (setDisabled) {
                (*button)->setIcon(QIcon(":/icon-reset-dark"));
                (*button)->setDisabled(true);
            } else {
                (*button)->setIcon(QIcon(":/icon-reset"));
            }
        });
    }

    this->setFocus();
}
