#include "topbar.h"

#include <QFile>

#include <QFont>
#include <QPalette>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

TopBar::TopBar(const QString *path, QObject *parent) : QObject(parent)
{
    QFile file;
    if (file.exists(*path+"db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(*path+"db.sqlite");
    } else {
        //emit db_error();
        qDebug() << "db_error";
    }

    setup_ui();
}

TopBar::~TopBar()
{
}

void TopBar::query_db(const QString &str, const bool getData)
{
    const QString category = "";
    const QString pornstar = "";
    const QString quality = "";
    const QString full = "";

    if (db.open()) {
        QSqlQuery query;
        if (getData) {
            query.prepare("SELECT id, Title, Cas, Category, Quality, Time, Full \
                          , HostCover, UrlCoverFront, UrlCoverBack, HostPreview, UrlPreview \
                          , HostTorrent, UrlTorrent, Scenes FROM Movies WHERE Title LIKE \
                          '%"+str+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE \
                          '%"+category+"%' AND Quality LIKE '%"+quality+"%' AND Full LIKE \
                          '%"+full+"%' ORDER BY Title ASC LIMIT 1000");
        } else {
            query.prepare("SELECT id FROM Movies WHERE Title LIKE \
                          '%"+str+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE \
                          '%"+category+"%' AND Quality LIKE '%"+quality+"%' AND Full LIKE \
                          '%"+full+"%' ORDER BY Title ASC LIMIT 1");
        }
        if (!query.exec()) {
            db.close();
            //db_error();
        } else {
            data.clear();
            for (int i = 0; query.next(); i++) {
                if (getData) {
                    data << query.value(0).toString() << query.value(1).toString() << query.value(2).toString()
                         << query.value(3).toString() << query.value(4).toString() << query.value(5).toString()
                         << query.value(6).toString() << query.value(7).toString() << query.value(8).toString()
                         << query.value(9).toString() << query.value(10).toString() << query.value(11).toString()
                         << query.value(12).toString() << query.value(13).toString() << query.value(14).toString();
                }
            }
            if (getData) {
                if (!data.isEmpty())
                    emit sendData(data);
            } else {
                if (!query.last())
                    qDebug() << "NO-MOVIES";
                else
                    qDebug() << "MOVIES";
            }

            db.close();
        }
    } else {
        //db_error();
    }
}

void TopBar::setup_ui()
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
    QObject::connect(finder, &QLineEdit::textChanged, [&] (const QString str) { query_db(str); });
    QObject::connect(finder, &QLineEdit::returnPressed, [=] {
        query_db((finder->selectAll(),finder->selectedText()), true);
        finder->deselect();
    });

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
