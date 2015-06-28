#include "pig.h"

#include <QDir>
#include <QDebug>//

PIG::PIG(QWidget *parent) : QWidget(parent)
{
#ifdef __linux__
    path = QDir::homePath()+"/.pig/";
#else
    path = "C:/PIG/.pig/";
#endif

    setupUi();
    authorization(false);
}

PIG::~PIG()
{
}

void PIG::authorization(const bool set)
{
    Password *pPassword = new Password(&path, set);
    QObject::connect(pPassword, &Password::ready, [=] {
        //pTopbar->group->setDisabled(true);
        layout->addWidget(pPassword->group);
    });
    QObject::connect(pPassword, &Password::finished, [=] { //TODO: REVISAR 'PUBLIC'.
        //pTopbar->group->setDisabled(false);              //COMPROBAR SI 'PPASSWORD' SE AGREGO AL LAYOUT ANTES DE SACARLO
        //pPassword->group->hide();
        //layout->removeWidget(pPassword->group);
        pPassword->deleteLater();
    });

    pPassword->check();
}

void PIG::showData(const QStringList &data)
{
    qDebug() << data[1];
}

void PIG::setupUi()
{
    QBrush b(QColor(0, 0, 0, 255));

    QPalette p;
    p.setBrush(QPalette::Active, QPalette::Base, b);
    p.setBrush(QPalette::Active, QPalette::Window, b);
    p.setBrush(QPalette::Disabled, QPalette::Base, b);
    p.setBrush(QPalette::Disabled, QPalette::Window, b);
    setPalette(p);

    pTopbar = new TopBar(&path, this);
    QObject::connect(pTopbar, &TopBar::sendData, [&] (const QStringList data) { showData(data); });

    layout = new QVBoxLayout(this);
    layout->addWidget(pTopbar->getGroup());
    layout->setAlignment(pTopbar->getGroup(), Qt::AlignTop);

    setLayout(layout);
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
    QObject::connect(pTopbar->category, &QPushButton::clicked, [=]() { qDebug() << "CLICKED"; });


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
        Password mPassword;
        if (mPassword.check(&plain))
            //update_handler();
            qDebug() << "x";
        else
            //emit sig_ret_password();
            qDebug() << "x";
    } else if (write) {
        Password mPassword;
        if (mPassword.write(&plain))
            //emit sig_ret_password(false, true);
            qDebug() << "x";
        else
            //emit sig_ret_password();
            qDebug() << "x";
    }
}
*/
