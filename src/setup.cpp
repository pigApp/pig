#include "setup.h"
#include "authorization.h"

#include <QTextStream>

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
            //QTimer::singleShot(100, this, SLOT(error()));
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
        //QTimer::singleShot(100, this, SLOT(error()));
    }

    Authorization *authorization = new Authorization(PIG_PATH, true, this);
    authorization->check();

    ui->setupUi(&data, &keep_covers, &keep_torrents, &keep_movies,
                &torrent_port_1, &torrent_port_2, authorization, this);

    QObject::connect (ui->radio_folder_covers, &QPushButton::pressed, [=] {
        *keep_covers = !*keep_covers;
        set_rc("KEEP_LOCAL_COPY_OF_COVERS", *keep_covers);
    });
    QObject::connect (ui->radio_folder_torrents, &QPushButton::pressed, [=] {
        *keep_torrents = !*keep_torrents;
        set_rc("KEEP_LOCAL_COPY_OF_TORRENTS", *keep_torrents);
    });
    QObject::connect (ui->radio_folder_movies, &QPushButton::pressed, [=] {
        *keep_movies = !*keep_movies;
        set_rc("KEEP_LOCAL_COPY_OF_MOVIES", *keep_movies);
    });

    QObject::connect (ui->button_folder_covers_clean, &QPushButton::pressed, [&] {
        clean_folder("covers");
        clean_folder("covers/back");
    });
    QObject::connect (ui->button_folder_torrents_clean, &QPushButton::pressed, [&] {
        clean_folder("torrent");
    });
    QObject::connect (ui->button_folder_movies_clean, &QPushButton::pressed, [&] {
        clean_folder("torrents/movies");
    });

    QObject::connect (ui->input_torrent_port_1, &QLineEdit::cursorPositionChanged, [&] {
        ui->input_torrent_port_1->setPalette(ui->palette_torrent_edit);
        if ((ui->input_torrent_port_1->text().toInt() == 6900)
            && (ui->input_torrent_port_2->text().toInt() == 6999))
            ui->button_torrent_ports_reset->setDisabled(true);
        else
            ui->button_torrent_ports_reset->setEnabled(true);
    });
    QObject::connect (ui->input_torrent_port_1, &QLineEdit::editingFinished, [=] {
        *torrent_port_1 = ui->input_torrent_port_1->text().toInt();
        set_rc("TORRENT_PORT_1", *torrent_port_1);
        ui->input_torrent_port_1->setPalette(ui->palette_torrent);
    });

    QObject::connect (ui->input_torrent_port_2, &QLineEdit::cursorPositionChanged, [&] {
        ui->input_torrent_port_2->setPalette(ui->palette_torrent_edit);
        if ((ui->input_torrent_port_1->text().toInt() == 6900)
            && (ui->input_torrent_port_2->text().toInt() == 6999))
            ui->button_torrent_ports_reset->setDisabled(true);
        else
            ui->button_torrent_ports_reset->setEnabled(true);
    });
    QObject::connect (ui->input_torrent_port_2, &QLineEdit::editingFinished, [=] {
        *torrent_port_2 = ui->input_torrent_port_2->text().toInt();
        set_rc("TORRENT_PORT_2", *torrent_port_2);
        ui->input_torrent_port_2->setPalette(ui->palette_torrent);
    });

    QObject::connect (ui->button_torrent_ports_reset, &QPushButton::pressed, [=] {
        *torrent_port_1 = 6900;
        *torrent_port_2 = 6999;
        set_rc("TORRENT_PORT_1", *torrent_port_1);
        set_rc("TORRENT_PORT_2", *torrent_port_2);
        ui->input_torrent_port_1->setText("6900");
        ui->input_torrent_port_2->setText("6999");
        ui->input_torrent_port_1->setPalette(ui->palette_torrent);
        ui->input_torrent_port_2->setPalette(ui->palette_torrent);
        ui->button_torrent_ports_reset->setDisabled(true);
    });

    QObject::connect (ui->button_contribute_torrents, &QPushButton::pressed, [=] {
        QDesktopServices::openUrl(QUrl("http://"+data[3]+data[4]));
    });
    QObject::connect (ui->button_contribute_code, &QPushButton::pressed, [=] {
        QDesktopServices::openUrl(QUrl("http://"+data[3]+data[5]));
    });
    QObject::connect (ui->button_contribute_bugs, &QPushButton::pressed, [=] {
        QDesktopServices::openUrl(QUrl("http://"+data[3]+data[6]));
    });
    QObject::connect (ui->button_contribute_bitcoins, &QPushButton::pressed, [&] {
        if (ui->label_contribute_bitcoins->text() == "BITCOINS")
            ui->label_contribute_bitcoins->setText("WALLET");
        else
            ui->label_contribute_bitcoins->setText("BITCOINS");
        ui->label_contribute_bitcoins_wallet->setVisible(ui->label_contribute_bitcoins_wallet->isHidden());
    });
    QObject::connect (ui->button_contribute_help, &QPushButton::pressed, [=] {
        QDesktopServices::openUrl(QUrl("http://"+data[3]+data[7]));
    });
}

Setup::~Setup()
{
    delete ui;
}

void Setup::set_rc(const QString &option, const QVariant &value)
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
    }
}

void Setup::clean_folder(const QString &folder)
{
    QDir dir(*_PIG_PATH+"/tmp/"+folder);
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    if (dir.exists())
        foreach (QString file, dir.entryList())
            dir.remove(file);
}

