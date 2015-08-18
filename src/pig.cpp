#include "pig.h"
#include "authorization.h"
#include "update.h"

#include <QDebug>//

PIG::PIG(QWidget *parent) :
    QWidget(parent),
    topbar(NULL),
    setup(NULL),
    view(NULL),
    keep_covers(true),
    keep_torrents(true),
    keep_movies(true),
    torrent_port_1(6900),
    torrent_port_2(6999),
    ui(new Ui::PIG)
{
    init();

    ui->setupUi(this);

    ui->b_minimize->installEventFilter(this);
    ui->b_close->installEventFilter(this);

    QObject::connect (ui->b_minimize, &QPushButton::released, [&] { showMinimized(); });
    QObject::connect (ui->b_close, &QPushButton::released, [&] { exit(0); });

    init_authorization();
}

PIG::~PIG()
{
    delete ui;
}

void PIG::init()
{
    QDir dir;
    if (!dir.exists(PIG_PATH+"/tmp"))
        if (!dir.mkdir(PIG_PATH+"/tmp"))
            error("CHECK PERMISSIONS ON "+PIG_PATH);
    if (!dir.exists(PIG_PATH+"/tmp/update"))
        if (!dir.mkdir(PIG_PATH+"/tmp/update"))
            error("CHECK PERMISSIONS ON "+PIG_PATH);
    if (!dir.exists(PIG_PATH+"/tmp/covers"))
        if (!dir.mkdir(PIG_PATH+"/tmp/covers"))
            error("CHECK PERMISSIONS ON "+PIG_PATH);
    if (!dir.exists(PIG_PATH+"/tmp/covers/back"))
        if (!dir.mkdir(PIG_PATH+"/tmp/covers/back"))
            error("CHECK PERMISSIONS ON "+PIG_PATH);
    if (!dir.exists(PIG_PATH+"/tmp/torrents"))
        if (!dir.mkdir(PIG_PATH+"/tmp/torrents"))
            error("CHECK PERMISSIONS ON "+PIG_PATH);
    if (!dir.exists(PIG_PATH+"/tmp/torrents/movies"))
        if (!dir.mkdir(PIG_PATH+"/tmp/torrents/movies"))
            error("CHECK PERMISSIONS ON "+PIG_PATH);

    QFile file;
    if (file.exists(PIG_PATH+"/db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PIG_PATH+"/db.sqlite");
    } else {
        error("DATABASE DO NOT EXISTS");
    }

    QHash<QString, QVariant> rc = get_rc();

    if (!rc.isEmpty()) {
        keep_covers = rc.value("KEEP_LOCAL_COPY_OF_COVERS").toBool();
        keep_torrents = rc.value("KEEP_LOCAL_COPY_OF_TORRENTS").toBool();
        keep_movies = rc.value("KEEP_LOCAL_COPY_OF_MOVIES").toBool();
        torrent_port_1 = rc.value("TORRENT_PORT_1").toInt();
        torrent_port_2 = rc.value("TORRENT_PORT_2").toInt();
    }
}

void PIG::init_authorization()
{
    Authorization *authorization = new Authorization(&PIG_PATH, false, this);

    QObject::connect (authorization, &Authorization::showWidget, [&] (QWidget *w) {
        ui->main_layout->addWidget(w);
    });
    QObject::connect (authorization, &Authorization::destroyed, [&] { init_update(); });

    authorization->check();
}

void PIG::init_update()
{
    Update *update = new Update(&PIG_PATH, &db, this);

    QObject::connect (update, &Update::showWidget, [&] (QWidget *w) {
        for (int i = 0; i < ui->main_layout->count(); i++)
            ui->main_layout->itemAt(i)->widget()->setDisabled(true);
        ui->main_layout->insertWidget(0, w);
        w->show();
    });
    QObject::connect (update, &Update::destroyed, [&] {
        for (int i = 0; i < ui->main_layout->count(); i++)
            ui->main_layout->itemAt(i)->widget()->setEnabled(true);
    });
    connect(update, SIGNAL(dbError(QString)), this, SLOT(error(QString)));

    init_topbar();
}

void PIG::init_topbar()
{
    topbar = new TopBar(&db, this);

    connect (topbar->getFinderObj(), SIGNAL(sendData(const QStringList*, const QString*)),
             this, SLOT(init_viewer(const QStringList*, const QString*)));
    connect (topbar->getButtonSetupObj(), SIGNAL(pressed()), this, SLOT(init_setup()));

    ui->main_layout->addWidget(topbar);
}

void PIG::init_viewer(const QStringList *data, const QString *filter)
{
    if (view == 0 && data != 0) {
        view = new View(&PIG_PATH, this);

        QObject::connect (view, &View::setFilterOnCovers, [&] {
            topbar->getFinderObj()->setFilterOnCovers();
        });
        QObject::connect (view, &View::setTopbarState, [&] (bool hide) {
            topbar->setHidden(hide);
        });

        ui->main_layout->addWidget(view);

        if (!topbar->getButtonSetupObj()->isEnabled())
            topbar->getButtonSetupObj()->setEnabled(true);
    }

    if (data != 0)
        view->get_covers(data);

    if (filter != 0)
        view->set_filter(filter);
}

void PIG::init_setup()
{
    if (setup == 0) {
        setup = new Setup(&PIG_PATH, &keep_covers, &keep_torrents, &keep_movies,
                          &torrent_port_1, &torrent_port_2, &db, this);
        connect (setup->getButtonBackObj(), SIGNAL(pressed()), this, SLOT(init_setup()));
        ui->main_layout->addWidget(setup);
        topbar->setHidden(true);
        view->hide();
    } else {
        ui->main_layout->removeWidget(setup);
        topbar->setHidden(false);
        view->show();
        setup->deleteLater();
        setup = NULL;
    }
}

void PIG::error(QString error)
{
    qDebug() << "ERROR: " << error; //TODO: HACER ESTO.
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
    if (obj == (QObject*)ui->b_minimize) {
        if (e->type() == QEvent::Enter) {
            ui->b_minimize->setIcon(QIcon(":/icon-minimize-dark"));
            return true;
        } else if (e->type() == QEvent::Leave) {
            ui->b_minimize->setIcon(QIcon(":/icon-minimize"));
            return true;
        } else {
            return false;
        }
    } else if (obj == (QObject*)ui->b_close) {
        if (e->type() == QEvent::Enter) {
            ui->b_close->setIcon(QIcon(":/icon-close-dark"));
            return true;
        } else if (e->type() == QEvent::Leave) {
            ui->b_close->setIcon(QIcon(":/icon-close"));
            return true;
        } else {
            return false;
        }
    } else {
        return QWidget::eventFilter(obj, e);
    }
}
