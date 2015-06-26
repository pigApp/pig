#include "topbar.h"

#include <QFile>

#include <QFont>
#include <QPalette>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

TopBar::TopBar(QObject *parent, const QString path) : QObject(parent)
{
    QFile file;
    if (file.exists(path+"db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(path+"db.sqlite");
    } else {
        //emit db_error();
        qDebug() << "db_error";
    }

    setupUi();
}

TopBar::~TopBar()
{
    delete group;
}

void TopBar::request(const QString str)
{
    const QString category = "";
    const QString pornstar = "";
    const QString quality = "";
    const QString full = "";

    data.clear();

    if (!str.isEmpty() && db.open()) {
        QSqlQuery query;
        query.prepare("SELECT id, Title, Cas, Category, Quality, Time, Full \
                      , HostCover, UrlCoverFront, UrlCoverBack, HostPreview, UrlPreview \
                      , HostTorrent, UrlTorrent, Scenes FROM Movies WHERE Title LIKE \
                      '%"+str+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE \
                      '%"+category+"%' AND Quality LIKE '%"+quality+"%' AND Full LIKE \
                      '%"+full+"%' ORDER BY Title ASC LIMIT 1000");
        if (!query.exec()) {
            db.close();
            //db_error();
        } else {
            //int nMovies = 0;
            for (int i = 0; query.next(); i++) { //TODO: DIVIDIR ENTRE PEDIDO DE EXISTENCIA Y PEDIDO DE DATOS
                const QString strId = query.value(0).toString();
                const QString strTitle = query.value(1).toString();
                const QString strCast = query.value(2).toString();
                const QString strCategories = query.value(3).toString();
                const QString strQuality = query.value(4).toString();
                const QString strTime = query.value(5).toString();
                const QString strFull = query.value(6).toString();
                const QString strHostCover = query.value(7).toString();
                const QString strUrlCoverFront = query.value(8).toString();
                const QString strUrlCoverBack = query.value(9).toString();
                const QString strHostPreview = query.value(10).toString();
                const QString strUrlPreview = query.value(11).toString();
                const QString strHostTorrent = query.value(12).toString();
                const QString strUrlTorrent = query.value(13).toString();
                const QString strScenes = query.value(14).toString();

                data << strId << strTitle << strCast << strCategories << strQuality
                     << strTime << strFull << strHostCover << strUrlCoverFront << strUrlCoverBack
                     << strHostPreview << strUrlPreview << strHostTorrent << strUrlTorrent << strScenes;
            }
            db.close();

            if (!query.last()) {
                //emit sig_ret_db(nMovies, dataMovies);
                qDebug() << "NULL";
            } else {
                //nMovies = query.at()+1;
                //emit sig_ret_db(nMovies, dataMovies);
                qDebug() << "OK";
            }
        }
    } else {
        //db_error();
    }
}

void TopBar::setupUi()
{
    group = new QGroupBox;
    group->setStyleSheet("QGroupBox{ border:0; }");
    group->setFlat(true);

    QFont f(":/font-global");
    f.setPointSize(24); //TODO: PASAR A PORCENTAJE
    f.setCapitalization(QFont::AllUppercase);
    f.setBold(true);

    QBrush b(QColor(0, 0, 0, 255));
    QBrush b1(QColor(255, 255, 255, 255));
    QBrush b2(QColor(255, 255, 255, 255));
    QBrush b3(QColor(255, 255, 255, 255));

    QPalette p;
    p.setBrush(QPalette::Active, QPalette::Text, b1);
    p.setBrush(QPalette::Active, QPalette::Base, b);
    p.setBrush(QPalette::Active, QPalette::Window, b);
    p.setBrush(QPalette::Active, QPalette::Highlight, b);
    p.setBrush(QPalette::Disabled, QPalette::Text, b);
    p.setBrush(QPalette::Disabled, QPalette::Base, b);
    p.setBrush(QPalette::Disabled, QPalette::Window, b);
    p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

    QPalette p1;
    p1.setBrush(QPalette::Active, QPalette::Button, b);
    p1.setBrush(QPalette::Active, QPalette::ButtonText, b2);
    p1.setBrush(QPalette::Active, QPalette::Base, b);
    p1.setBrush(QPalette::Active, QPalette::Window, b);
    p1.setBrush(QPalette::Active, QPalette::Highlight, b);
    p1.setBrush(QPalette::Disabled, QPalette::Button, b);
    p1.setBrush(QPalette::Disabled, QPalette::ButtonText, b);
    p1.setBrush(QPalette::Disabled, QPalette::Base, b);
    p1.setBrush(QPalette::Disabled, QPalette::Window, b);
    p1.setBrush(QPalette::Disabled, QPalette::Highlight, b);

    QPalette p2;
    p2.setBrush(QPalette::Active, QPalette::Button, b);
    p2.setBrush(QPalette::Active, QPalette::ButtonText, b3);
    p2.setBrush(QPalette::Active, QPalette::Base, b);
    p2.setBrush(QPalette::Active, QPalette::Window, b);
    p2.setBrush(QPalette::Active, QPalette::Highlight, b);
    p2.setBrush(QPalette::Disabled, QPalette::Button, b);
    p2.setBrush(QPalette::Disabled, QPalette::ButtonText, b);
    p2.setBrush(QPalette::Disabled, QPalette::Base, b);
    p2.setBrush(QPalette::Disabled, QPalette::Window, b);
    p2.setBrush(QPalette::Disabled, QPalette::Highlight, b);

    QLineEdit *finder = new QLineEdit(group);
    finder->setFont(f);
    finder->setPalette(p);

    connect(finder, SIGNAL(textChanged(const QString)), this, SLOT(request(const QString)));
    QObject::connect(finder, &QLineEdit::returnPressed, [&] { emit sendMoviesData(data); });

    QPushButton *category = new QPushButton("CATEGORY", group);
    category->setFont(f);
    category->setPalette(p1);
    category->setFlat(true);

    QPushButton *pornstar = new QPushButton("PORNSTAR", group);
    pornstar->setFont(f);
    pornstar->setPalette(p2);
    pornstar->setFlat(true);

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(finder);
    layout->addWidget(category);
    layout->addWidget(pornstar);

    group->setLayout(layout);
}
