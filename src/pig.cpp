#include "pig.h"
#include "authorization.h"
#include "update.h"

#include <QDir>

#include <QDebug>//
#include <QTimer>//

PIG::PIG(QWidget *parent) :
    QWidget(parent),
    topbar(NULL),
    setup(NULL),
    view(NULL),
    ui(new Ui::PIG)
{
#ifdef __linux__
    PIG_PATH = QDir::homePath()+"/.pig";
#else
    PIG_PATH = "C:/PIG/.pig";
#endif
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

    QFile file;
    if (file.exists(PIG_PATH+"/db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PIG_PATH+"/db.sqlite");
    } else {
        error("DATABASE DO NOT EXISTS");
    }

    ui->setupUi(this);

    init_authorization();
}

PIG::~PIG()
{
    delete ui;
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

    connect (topbar->getFinderObject(), SIGNAL(sendData(const QStringList*, const QString*)),
             this, SLOT(init_viewer(const QStringList*, const QString*)));
    connect (topbar->getBtnSetupObject(), SIGNAL(released()), this, SLOT(init_setup()));

    ui->main_layout->addWidget(topbar);
}

void PIG::init_viewer(const QStringList *data, const QString *filter)
{
    if (view == 0 && data != 0) {
        view = new View(&PIG_PATH, this);

        QObject::connect (view, &View::setFilterOnCovers, [&] {
            topbar->getFinderObject()->setFilterOnCovers();
        });
        QObject::connect (view, &View::setTopbarState, [&] (bool hide) {
            topbar->setHidden(hide);
        });

        ui->main_layout->addWidget(view);
    }

    if (data != 0)
        view->get(data);

    if (filter != 0)
        view->set_filter(filter);
}

void PIG::init_setup()
{
    if (setup == 0) {
        setup = new Setup(&PIG_PATH, &db, this);
        ui->main_layout->addWidget(setup);
        topbar->setHidden(true);
        view->hide();
        //QTimer::singleShot(2000, this, SLOT(init_setup()));
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

























/*
TODO
*/


/*
IMAGEN
    QLabel *logo;
    logo = new QLabel;
    logo->setPixmap(QPixmap(":/img-background"));


FIND
void TopBar::find(const QString userInput, const QString category, const QString pornstar
    , const QString quality, const QString full)


LAMBDA
    QObject::connect(topbar->category, &QPushButton::clicked, [=]() { qDebug() << "CLICKED"; });
    QObject::connect(topbar, &TopBar::sendData, [&] (const QStringList &data) { showData(data); });
    QObject::connect(topbar, &TopBar::addGroup, [&] (QGroupBox *filterGroup) { groupsHandler(&filterGroup); });//{ main_layout->addWidget(filterGroup); });

    QObject::connect(auth, &Authorization::ready, [=] { //TODO: PASAR EL GRUPO EN LA SEÑAL.
        main_layout->addWidget(auth->getGroup());
        topbar->getGroup()->setDisabled(true);
    });
    QObject::connect(auth, &Authorization::finished, [=] {
        if (auth->getGroup() != NULL) {
            auth->getGroup()->hide();
            main_layout->removeWidget(auth->getGroup());
            topbar->getGroup()->setDisabled(false);
        }
        auth->deleteLater();
    });


AUTHORIZATION
void PIG::authorization(const bool require, const QString plain
    , const bool check, const bool write)
{
    if (require) {
#ifdef __linux__
    const QString target = QDir::homePath()+"/.pig/.pd";
#else
    const QString target = "C:/PIG/.pig/.pd";
#endif
       QFile file;
       if (file.exists(target))
           //emit sig_ret_password(true);
           qDebug() << "x";
       else
           //update_handler();
           qDebug() << "x";
    } else if (check) {
        Authorization mPassword;
        if (mPassword.check(&plain))
            //update_handler();
            qDebug() << "x";
        else
            //emit sig_ret_password();
            qDebug() << "x";
    } else if (write) {
        Authorization mPassword;
        if (mPassword.write(&plain))
            //emit sig_ret_password(false, true);
            qDebug() << "x";
        else
            //emit sig_ret_password();
            qDebug() << "x";
    }
}
*/
