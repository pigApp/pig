#include "topbar.h"

#include <QFile>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>

TopBar::TopBar(const QString *PIG_PATH, QObject *parent) : QObject(parent)
{
    QFile file;
    if (file.exists(*PIG_PATH+"db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(*PIG_PATH+"db.sqlite");
    } else {
        //emit db_error();
        qDebug() << "db_error";
    }

    setup_ui();
}

TopBar::~TopBar()
{
}

QGroupBox *TopBar::filterGroup(const QString &filter, const QStringList &filterData)
{
    QGroupBox *filterGroup = new QGroupBox;
    filterGroup->setStyleSheet("QGroupBox{ border:0; }");
    filterGroup->setFlat(true);

    QDesktopWidget desk; //TODO: MOVERLO AL CONSTRUCTOR DE PIG.

    QFont f(":/font-global");
    f.setPointSize(24); //TODO: PASAR A PORCENTAJE
    f.setCapitalization(QFont::AllUppercase);
    f.setBold(true);

    QBrush b(QColor(0, 0, 0, 255));
    QBrush b1(QColor(255, 255, 255, 255));

    QPalette p1;
    p1.setBrush(QPalette::Active, QPalette::Button, b);
    p1.setBrush(QPalette::Active, QPalette::ButtonText, b1);
    p1.setBrush(QPalette::Active, QPalette::Base, b);
    p1.setBrush(QPalette::Active, QPalette::Window, b);
    p1.setBrush(QPalette::Active, QPalette::Highlight, b);
    p1.setBrush(QPalette::Disabled, QPalette::Button, b);
    p1.setBrush(QPalette::Disabled, QPalette::ButtonText, b);
    p1.setBrush(QPalette::Disabled, QPalette::Base, b);
    p1.setBrush(QPalette::Disabled, QPalette::Window, b);
    p1.setBrush(QPalette::Disabled, QPalette::Highlight, b);

    QString icon_path;
    if (filter == "Categories")
        icon_path = ":/img-cat-";
    else
        icon_path = ":/img-star-";

    QPushButton *btn[filterData.count()];
    QGridLayout *filterLayout = new QGridLayout(filterGroup);

    for (int i = 0; i < filterData.count(); ++i) {
        btn[i] = new QPushButton(filterGroup);
        btn[i]->setFont(f);
        btn[i]->setPalette(p1);
        btn[i]->setFlat(true);
        btn[i]->setIcon(QIcon(icon_path+filterData[i]));
        btn[i]->setIconSize(QSize((desk.width()/4), (desk.height()/4)));

        if (i < 7) {
            filterLayout->addWidget(btn[i], 0, i + 1); //TODO: HACER EL CALCULO BIEN
        } else if (i > 6 && i < 14) {
            filterLayout->addWidget(btn[i], 1, (i-7) + 1);
        } else if (i > 13 && i < 21) {
            filterLayout->addWidget(btn[i], 2, (i-14) + 1);
        } else if (i > 20 && i < 28){
            filterLayout->addWidget(btn[i], 3, (i-21) + 1);
        } else if (i > 27 && i < 35){
            filterLayout->addWidget(btn[i], 4, (i-28) + 1);
        }
    }

    filterGroup->setLayout(filterLayout);

    return filterGroup;
}

void TopBar::query(const QString &str, const bool getData, const bool getFilter)
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
        } else if (getFilter) {
            query.prepare("SELECT "+str+" FROM FiltersData");
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
            QStringList data;
            QStringList filterData;

            for (int i = 0; query.next(); i++) {
                if (getData) {
                    data << query.value(0).toString() << query.value(1).toString() << query.value(2).toString()
                         << query.value(3).toString() << query.value(4).toString() << query.value(5).toString()
                         << query.value(6).toString() << query.value(7).toString() << query.value(8).toString()
                         << query.value(9).toString() << query.value(10).toString() << query.value(11).toString()
                         << query.value(12).toString() << query.value(13).toString() << query.value(14).toString();
                } else if (getFilter) {
                    filterData << query.value(0).toString().split(",");
                }
            }
            db.close();

            if (getData && !data.isEmpty()) {
                emit sendData(data);
            } else if (getFilter) {
                emit sendGroup(filterGroup(str, filterData), true);
            } else {
                if (!query.last())
                    qDebug() << "NO-MOVIES";
                else
                    qDebug() << "MOVIES";
            }
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

    QLineEdit *input = new QLineEdit(group);
    input->setFont(f);
    input->setPalette(p);
    QObject::connect(input, &QLineEdit::textChanged, [&] (const QString str) { query(str); });
    QObject::connect(input, &QLineEdit::returnPressed, [=] {
        query((input->selectAll(),input->selectedText()), true);
        input->deselect();
    });

    QPushButton *btnCategory = new QPushButton("CATEGORY", group);
    btnCategory->setFont(f);
    btnCategory->setPalette(p1);
    btnCategory->setFlat(true);
    QObject::connect(btnCategory, &QPushButton::pressed, [&] { query("Categories", false, true); });

    QPushButton *btnPornstar = new QPushButton("PORNSTAR", group);
    btnPornstar->setFont(f);
    btnPornstar->setPalette(p2);
    btnPornstar->setFlat(true);
    QObject::connect(btnPornstar, &QPushButton::pressed, [&] { query("Pornstars", false, true); });

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(input);
    layout->addWidget(btnCategory);
    layout->addWidget(btnPornstar);

    group->setLayout(layout);
}
