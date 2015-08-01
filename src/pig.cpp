#include "pig.h"
#include "authorization.h"
#include "update.h"

#include <QDir>

#include <QDebug>//

PIG::PIG(QWidget *parent) :
    QWidget(parent),
    view(NULL),
    setup(NULL),
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
            qDebug() << "error"; //error()
    if (!dir.exists(PIG_PATH+"/tmp/covers"))
        if (!dir.mkdir(PIG_PATH+"/tmp/covers"))
            qDebug() << "error"; //error()
    if (!dir.exists(PIG_PATH+"/tmp/covers/back"))
        if (!dir.mkdir(PIG_PATH+"/tmp/covers/back"))
            qDebug() << "error"; //error()

    QFile file;
    if (file.exists(PIG_PATH+"/db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PIG_PATH+"/db.sqlite");
    } else {
        //emit db_error();
    }

    ui->setupUi(this);

    init_authorization(false);
}

PIG::~PIG()
{
    delete ui;
}

void PIG::init_authorization(bool set)
{
    Authorization *authorization = new Authorization(&PIG_PATH, set, this);

    QObject::connect (authorization, &Authorization::showWidget, [&] (QWidget *w) {
        ui->layout_main->addWidget(w);
    });
    QObject::connect (authorization, &Authorization::destroyed, [&] { init_update(); });

    authorization->check();
}

void PIG::init_update()
{
    Update *update = new Update(&PIG_PATH, &db, this);
    Q_UNUSED(update);

    init_topbar();
}

void PIG::init_topbar()
{
    topbar = new TopBar(&db, this);

    connect (topbar->getFinderObject(), SIGNAL(sendData(const QStringList*, const QString*)),
             this, SLOT(init_viewer(const QStringList*, const QString*)));
    connect (topbar->getBtnSetupObject(), SIGNAL(released()), this, SLOT(init_setup()));

    ui->layout_main->addWidget(topbar);
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

        ui->layout_main->addWidget(view);
    }

    if (data != 0)
        view->get(data);

    if (filter != 0)
        view->set_filter(filter);
}

void PIG::init_setup()
{
    if (setup == 0) {
        setup = new Setup(this);
        ui->layout_main->replaceWidget(view, setup);
        view->hide();
    } else {
        ui->layout_main->replaceWidget(setup, view);
        view->show();
        setup->deleteLater();
        setup = NULL;
    }
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
    QObject::connect(topbar, &TopBar::addGroup, [&] (QGroupBox *filterGroup) { groupsHandler(&filterGroup); });//{ layout_main->addWidget(filterGroup); });

    QObject::connect(auth, &Authorization::ready, [=] { //TODO: PASAR EL GRUPO EN LA SEÑAL.
        layout_main->addWidget(auth->getGroup());
        topbar->getGroup()->setDisabled(true);
    });
    QObject::connect(auth, &Authorization::finished, [=] {
        if (auth->getGroup() != NULL) {
            auth->getGroup()->hide();
            layout_main->removeWidget(auth->getGroup());
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
