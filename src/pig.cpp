#include "pig.h"
#include "authorization.h"
#include "update.h"

#include <QDir>
#include <QDebug>//

PIG::PIG(QWidget *parent) :
    QWidget(parent),
    view(NULL),
    ui(new Ui::PIG)
{
#ifdef __linux__
    PIG_PATH = QDir::homePath()+"/.pig";
#else
    PIG_PATH = "C:/PIG/.pig";
#endif

    QFile file;
    if (file.exists(PIG_PATH+"/db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PIG_PATH+"/db.sqlite");
    } else {
        //emit db_error();
        qDebug() << "db_error";
    }


    ui->setupUi(this);

    topbar = new TopBar(&db, this);

    connect (topbar->getPtrObject(), SIGNAL(sendData(const QStringList*)), this, SLOT(viewer(const QStringList*)));

    ui->mainLayout->addWidget(topbar);
    ui->mainLayout->setAlignment(topbar, Qt::AlignTop);

    authorization(false);
}

PIG::~PIG()
{
    delete ui;
}

void PIG::authorization(bool set)
{
    Auth *auth = new Auth(&PIG_PATH, set, this);

    connect (auth, SIGNAL(sendWidget(QWidget*, bool)), this, SLOT(widgetsHandler(QWidget*, bool)));
    QObject::connect (auth, &Auth::destroyed, [&] { update(); });

    auth->check();
}

void PIG::update()
{
    Update *update = new Update(&PIG_PATH, &db, this);

    connect (update, SIGNAL(sendWidget(QWidget*, bool)), this, SLOT(widgetsHandler(QWidget*, bool))); //TODO: PROBAR NO USAR 'widgetsHandler'.
}

void PIG::viewer(const QStringList *data)
{
    if (view == NULL) {
        view = new View(&PIG_PATH, this);
        ui->mainLayout->addWidget(view);
    }

    view->get(data);
}

void PIG::widgetsHandler(QWidget *w, bool add)
{
    if (add) {
        ui->mainLayout->addWidget(w);
        //topbar->getGroup()->setDisabled(true);
    } else {
        if (w != NULL) {
            ui->mainLayout->removeWidget(w);
            w->deleteLater();
            //topbar->getGroup()->setDisabled(false);
        }
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
    QObject::connect(topbar, &TopBar::addGroup, [&] (QGroupBox *filterGroup) { groupsHandler(&filterGroup); });//{ mainLayout->addWidget(filterGroup); });

    QObject::connect(auth, &Auth::ready, [=] { //TODO: PASAR EL GRUPO EN LA SEÑAL.
        mainLayout->addWidget(auth->getGroup());
        topbar->getGroup()->setDisabled(true);
    });
    QObject::connect(auth, &Auth::finished, [=] {
        if (auth->getGroup() != NULL) {
            auth->getGroup()->hide();
            mainLayout->removeWidget(auth->getGroup());
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
        Auth mPassword;
        if (mPassword.check(&plain))
            //update_handler();
            qDebug() << "x";
        else
            //emit sig_ret_password();
            qDebug() << "x";
    } else if (write) {
        Auth mPassword;
        if (mPassword.write(&plain))
            //emit sig_ret_password(false, true);
            qDebug() << "x";
        else
            //emit sig_ret_password();
            qDebug() << "x";
    }
}
*/
