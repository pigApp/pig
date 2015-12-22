#include "pig.h"
#include "authorization.h"
#include "update.h"

#include <QDebug>//

PIG::PIG(QWidget *parent) :
    QWidget(parent),
    topbar(NULL),
    setup(NULL),
    view(NULL),
    //torrent(NULL),
    player(NULL),
    error(NULL),
    keep_covers(true),
    keep_torrents(true),
    keep_movies(true),
    torrent_port_1(6900),
    torrent_port_2(6999),
    ui(new Ui::PIG)
{
    ui->setupUi(this);
    ui->b_back->installEventFilter(this);
    ui->b_minimize->installEventFilter(this);
    ui->b_quit->installEventFilter(this);

    QObject::connect (ui->b_minimize, &QPushButton::released, [&] { showMinimized(); });
    QObject::connect (ui->b_quit, &QPushButton::released, [&] { close(); });

    sc_quit = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
    sc_back = new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(init_setup()));
    sc_back->setEnabled(false);

    if (init())
        init_authorization();
}

PIG::~PIG()
{
    delete ui;
    ui = NULL;
}

bool PIG::init()
{
    QDir dir;
    if (!dir.exists(PIG_PATH+"/tmp"))
        if (!dir.mkdir(PIG_PATH+"/tmp")) {
            init_error("CHECK PERMISSIONS ON "+PIG_PATH);
            return false;
        }
    if (!dir.exists(PIG_PATH+"/tmp/update"))
        if (!dir.mkdir(PIG_PATH+"/tmp/update")) {
            init_error("CHECK PERMISSIONS ON "+PIG_PATH);
            return false;
        }
    if (!dir.exists(PIG_PATH+"/tmp/covers"))
        if (!dir.mkdir(PIG_PATH+"/tmp/covers")) {
            init_error("CHECK PERMISSIONS ON "+PIG_PATH);
            return false;
        }
    if (!dir.exists(PIG_PATH+"/tmp/covers/back"))
        if (!dir.mkdir(PIG_PATH+"/tmp/covers/back")) {
            init_error("CHECK PERMISSIONS ON "+PIG_PATH);
            return false;
        }
    if (!dir.exists(PIG_PATH+"/tmp/torrents"))
        if (!dir.mkdir(PIG_PATH+"/tmp/torrents")) {
            init_error("CHECK PERMISSIONS ON "+PIG_PATH);
            return false;
        }
    if (!dir.exists(PIG_PATH+"/tmp/torrents/movies"))
        if (!dir.mkdir(PIG_PATH+"/tmp/torrents/movies")) {
            init_error("CHECK PERMISSIONS ON "+PIG_PATH);
            return false;
        }

    QFile file;
    if (file.exists(PIG_PATH+"/db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PIG_PATH+"/db.sqlite");
    } else {
        init_error("DATABASE DO NOT EXISTS");
        return false;
    }

    QHash<QString, QVariant> rc = get_rc();
    if (!rc.isEmpty()) {
        keep_covers = rc.value("KEEP_LOCAL_COPY_OF_COVERS").toBool();
        keep_torrents = rc.value("KEEP_LOCAL_COPY_OF_TORRENTS").toBool();
        keep_movies = rc.value("KEEP_LOCAL_COPY_OF_MOVIES").toBool();
        torrent_port_1 = rc.value("TORRENT_PORT_1").toInt();
        torrent_port_2 = rc.value("TORRENT_PORT_2").toInt();
    }

    return true;
}

void PIG::init_authorization()
{
    Authorization *authorization = new Authorization(&PIG_PATH, false, this);

    QObject::connect (authorization, &Authorization::sendWidget, [&] (QWidget *w) {
        ui->main_layout->addWidget(w);
    });
    QObject::connect (authorization, &Authorization::destroyed, [&] { init_update(); });

    authorization->check();
}

void PIG::init_update()
{
    Update *update = new Update(&PIG_PATH, &db, this);

    QObject::connect (update, &Update::sendWidget, [&] (QWidget *w) {
        for (int i = 0; i < ui->main_layout->count(); i++)
            ui->main_layout->itemAt(i)->widget()->setDisabled(true);

        ui->main_layout->insertWidget(0, w);
        w->show();
    });
    QObject::connect (update, &Update::destroyed, [&] {
        if (ui != 0)
            for (int i = 0; i < ui->main_layout->count(); i++)
                ui->main_layout->itemAt(i)->widget()->setEnabled(true);
    });

    init_topbar();
}

void PIG::init_topbar()
{
    topbar = new TopBar(&db, this);

    connect (topbar->getFinderObj(), SIGNAL(sendData(const QStringList*, const QStringList*)),
             this, SLOT(init_view(const QStringList*, const QStringList*)));
    connect (topbar->getFinderObj(), SIGNAL(sendError(QString)), this, SLOT(init_error(QString)));
    connect (topbar->getButtonSetupObj(), SIGNAL(released()), this, SLOT(init_setup()));

    ui->main_layout->addWidget(topbar);
}

void PIG::init_view(const QStringList *data, const QStringList *filter)
{
    if (view == 0) {
        if (data != 0) {
            view = new View(&PIG_PATH, &ui->b_back, this);

            QObject::connect (view, &View::sendTopbarState, [&] (bool setHidden) {
                topbar->setHidden(setHidden);
            });
            connect (view, SIGNAL(sendTorrentData(const int&, const QStringList**, const int&, int)),
                     this, SLOT(init_movie(const int&, const QStringList**, const int&, int)));

            ui->main_layout->addWidget(view);
        }
    }

    if (view != 0) {
        if (data != 0)
            view->get_covers(data);

        if (filter != 0)
            view->set_filter(filter);
    }
}

void PIG::init_movie(const int &ID, const QStringList **data, const int &sizeData, int scene)
{
    test_path = "http://abv.cdn.vizplay.org/v/4fca0c95d17ef9371222670af35f55b1.mp4?st=wn5jhAxMLwkJu4LuGD0Q5w&hash=Apw4fFvjT400DOejXIrqQA";//"/home/lxfb/.pig/tmp/torrents/movies/test.mp4";//

    player = new Player(&test_path, this);

    view->hide();
    ui->main_layout->addWidget(player);

    torrent = new Torrent(&PIG_PATH, &((**data)[(ID * sizeData) + 16]), &(**data)[(ID * sizeData) + 17],
                          &(**data)[(ID * sizeData + 18)], scene, &player);
}

void PIG::init_setup()
{
    if (setup == 0) {
        setup = new Setup(&PIG_PATH, &keep_covers, &keep_torrents, &keep_movies,
                          &torrent_port_1, &torrent_port_2, &db, this);

        connect (setup, SIGNAL(sendError(QString)), this, SLOT(init_error(QString)));
        if (view != 0) {
            connect (setup, SIGNAL(folderCoversReset()), view, SLOT(reset_local_covers()));
            view->hide();
        }

        ui->main_layout->addWidget(setup);

        topbar->setHidden(true);

        connect (ui->b_back, SIGNAL(pressed()), this, SLOT(init_setup()));
        ui->b_back->show();

        sc_back->setEnabled(true);
    } else {
        ui->main_layout->removeWidget(setup);

        if (view != 0)
            view->show();

        topbar->setHidden(false);

        ui->b_back->disconnect();
        ui->b_back->hide();

        sc_back->setEnabled(false);

        setup->deleteLater();
        setup = NULL;
    }
}

void PIG::init_error(QString errorMsg)
{
    if ((ui != 0) && (error == 0)) {
        sc_back->setEnabled(false);

        ui->b_back->hide();
        ui->b_minimize->hide();
        ui->b_quit->hide();

        for (int i = 0; i < ui->main_layout->count(); i++)
            ui->main_layout->itemAt(i)->widget()->hide();

        error = new Error(&errorMsg, this);
        ui->main_layout->insertWidget(0, error, Qt::AlignCenter);
    }
}

QHash<QString, QVariant> PIG::get_rc()
{
    QHash<QString, QVariant> rc;

    QFile file(PIG_PATH+"/.pigrc");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = QString(file.readLine()).toUtf8();
            if (line.contains("="))
                rc.insert(line.section("=", 0, 0), line.section("=", 1, 1).simplified());
        }
        file.close();
    }

    return rc;
}

bool PIG::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == (QObject*)ui->b_back) {
        if (e->type() == QEvent::Enter) {
            ui->b_back->setIcon(QIcon(":/icon-back-dark"));
            return true;
        } else if (e->type() == QEvent::Leave) {
            ui->b_back->setIcon(QIcon(":/icon-back"));
            return true;
        } else {
            return false;
        }
    } else if (obj == (QObject*)ui->b_minimize) {
        if (e->type() == QEvent::Enter) {
            ui->b_minimize->setIcon(QIcon(":/icon-minimize-dark"));
            return true;
        } else if (e->type() == QEvent::Leave) {
            ui->b_minimize->setIcon(QIcon(":/icon-minimize"));
            return true;
        } else {
            return false;
        }
    } else if (obj == (QObject*)ui->b_quit) {
        if (e->type() == QEvent::Enter) {
            ui->b_quit->setIcon(QIcon(":/icon-quit-dark"));
            return true;
        } else if (e->type() == QEvent::Leave) {
            ui->b_quit->setIcon(QIcon(":/icon-quit"));
            return true;
        } else {
            return false;
        }
    } else {
        return QWidget::eventFilter(obj, e);
    }
}
