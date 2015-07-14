#include "pig.h"
#include "authorization.h"
#include "update.h"
#include "viewer.h"

#include <QDir>
#include <QDebug>//

PIG::PIG(QWidget *parent) : QWidget(parent)
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

    setup_ui();
    authorization(false);
}

PIG::~PIG()
{
}

void PIG::authorization(bool set)
{
    Auth *auth = new Auth(&PIG_PATH, set, this);

    connect (auth, SIGNAL(sendGroup(QGroupBox*, bool)), this, SLOT(groupHandler(QGroupBox*, bool)));
    connect (auth, SIGNAL(finished()), auth, SLOT(deleteLater()), Qt::QueuedConnection);
    QObject::connect (auth, &Auth::finished, [&] { if (!set) viewer(); });//update();

    auth->check();
}

void PIG::update()
{
    Update *update = new Update(&PIG_PATH, &db, this);

    connect (update, SIGNAL(sendGroup(QGroupBox*, bool)), this, SLOT(groupHandler(QGroupBox*, bool)));
    connect (update, SIGNAL(finished()), update, SLOT(deleteLater()), Qt::QueuedConnection);

    viewer();
}

//void PIG::viewer(const QStringList &data)
void PIG::viewer()
{
    //if (data == NULL) {
        test << "i.cubeupload.com";
        test << "/xj5Msc.jpg";
        test << "a.jpg";

        test << "i.cubeupload.com";
        test << "/Empeai.jpg";
        test << "b.jpg";

        test << "i.cubeupload.com";
        test << "/NhZ15k.jpg";
        test << "c.jpg";

        test << "i.cubeupload.com";
        test << "/CR9eLw.jpg";
        test << "d.jpg";

        test << "i.cubeupload.com";
        test << "/VUke0K.jpg";
        test << "e.jpg";

        test << "i.cubeupload.com";
        test << "/vsiE1o.jpg";
        test << "f.jpg";

        test << "i.cubeupload.com";
        test << "/1gO1Gr.jpg";
        test << "g.jpg";

        test << "i.cubeupload.com";
        test << "/kojoX1.jpg";
        test << "h.jpg";

        test << "i.cubeupload.com";
        test << "/iTIX6n.jpg";
        test << "i.jpg";

        test << "i.cubeupload.com";
        test << "/kojoX1.jpg";
        test << "j.jpg";
    //}

    Viewer *view = new Viewer(&PIG_PATH, this);
    connect (view, SIGNAL(sendGroup(QGroupBox*, bool)), this, SLOT(groupHandler(QGroupBox*, bool)));

    view->get(test);
}

void PIG::groupHandler(QGroupBox *group, bool add)
{
    if (add) {
        mainLayout->addWidget(group);
        //topbar->getGroup()->setDisabled(true);
    } else {
        if (group != NULL) {
            group->hide();
            mainLayout->removeWidget(group);
            //topbar->getGroup()->setDisabled(false);
        }
    }
}

void PIG::setup_ui()
{
    QBrush b(QColor(10, 10, 10, 255));

    QPalette p;
    p.setBrush(QPalette::Active, QPalette::Base, b);
    p.setBrush(QPalette::Active, QPalette::Window, b);
    p.setBrush(QPalette::Disabled, QPalette::Base, b);
    p.setBrush(QPalette::Disabled, QPalette::Window, b);
    setPalette(p);

    topbar = new TopBar(&db, this);

    //connect (topbar, SIGNAL(sendData(const QStringList&)), this, SLOT(showData(const QStringList&)));
    //connect (topbar, SIGNAL(sendGroup(QGroupBox*, bool)), this, SLOT(groupHandler(QGroupBox*, bool)));

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(topbar);
    mainLayout->setAlignment(topbar, Qt::AlignTop);

    setLayout(mainLayout);
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
