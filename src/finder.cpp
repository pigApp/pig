#include "finder.h"

#include <QDebug>//

Finder::Finder(QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    _db(db),
    ui(new Ui::Finder)
{
    ui->setupUi(this);

    connect (ui->btnFilter, SIGNAL(pressed()), this, SLOT(filter_handler()));
    QObject::connect (ui->input, &QLineEdit::textChanged, [&] (const QString str) { query(str); });
    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        data.clear();
        emit sendData(query((ui->input->selectAll(), ui->input->selectedText()), NULL, true));
        ui->input->deselect();
    });
}

Finder::~Finder()
{
    delete ui;
}

QStringList *Finder::query(const QString &str, const QString &category,
                           const bool &getData, const bool &getFilter)
{
    const QString pornstar = "";
    const QString quality = "";
    const QString full = "";

    if (_db->open()) {
        QSqlQuery query;

        if (getData) {
            query.prepare("SELECT id, Title, Cas, Category, Quality, Time, Full \
                          , HostCover, UrlCoverFront, UrlCoverBack, HostPreview, UrlPreview \
                          , HostTorrent, UrlTorrent, Scenes FROM Movies WHERE Title LIKE \
                          '"+str+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE \
                          '%"+category+"%' AND Quality LIKE '%"+quality+"%' AND Full LIKE \
                          '%"+full+"%' ORDER BY Title ASC LIMIT 1000");
        } else if (getFilter) {
            query.prepare("SELECT "+str+" FROM FiltersData");
        } else {
            query.prepare("SELECT id FROM Movies WHERE Title LIKE \
                          '"+str+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE \
                          '%"+category+"%' AND Quality LIKE '%"+quality+"%' AND Full LIKE \
                          '%"+full+"%' ORDER BY Title ASC LIMIT 1");
        }

        if (!query.exec()) {
            (_db)->close();
            //db_error();
        } else {

            for (int i = 0; query.next(); i++) {
                if (getData) {
                    data.append(query.value(0).toString());
                    data.append(query.value(1).toString());
                    data.append(query.value(2).toString());
                    data.append(query.value(3).toString());
                    data.append(query.value(4).toString());
                    data.append(query.value(5).toString());
                    data.append(query.value(6).toString());
                    data.append(query.value(7).toString());
                    data.append(query.value(8).toString());
                    data.append(query.value(9).toString());
                    data.append(query.value(10).toString());
                    data.append(query.value(11).toString());
                    data.append(query.value(12).toString());
                    data.append(query.value(13).toString());
                    data.append(query.value(14).toString());
                } else if (getFilter) {
                    filter << query.value(0).toString().split(",");
                }
            }

            (_db)->close();

            if (getData && !data.isEmpty()) {
                mFilterOnCovers = true;
                return &data;
            } else if (getFilter) {
                return &filter;
            } else {
                if (!query.last())
                    qDebug() << "NO MOVIE";
                else
                    qDebug() << "MOVIE";
            }
        }
    } else {
        //db_error();
    }

    return 0;
}


void Finder::filter_handler()
{
    if (ui->btn_categories_vector.isEmpty()) {
        const QStringList *categories = query("Categories", NULL, false, true);

        ui->setupFilterUi(this);

        QFont f(":/font-global");
        f.setPointSize(12); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(60, 60, 60, 255));
        QBrush b2(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b1);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Active, QPalette::Text, b2);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b1);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        //for (int i = 0; i < (*categories).size(); i++) {// ESTE
        for (int i = 0; i < 20; i++) {
            ui->btn_categories_vector.push_back(new QPushButton((*categories)[i], ui->groupFilter));
            ui->btn_categories_vector.last()->setFont(f);
            ui->btn_categories_vector.last()->setPalette(p);
            ui->btn_categories_vector.last()->setFlat(true);

            QObject::connect (ui->btn_categories_vector.last(), &QPushButton::pressed, [=] {
                if (!mFilterOnCovers) {
                    data.clear();
                    emit sendData(query(NULL, (*categories)[i], true, false));
                    ui->input->deselect();
                } else {
                    emit sendData(NULL, &(*categories)[i]);
                }
            });

            ui->layoutFilter->addWidget(ui->btn_categories_vector.last());
        }
    } else {
        ui->groupFilter->setEnabled(!ui->groupFilter->isEnabled());
        ui->groupFilter->setVisible(!ui->groupFilter->isVisible());
    }
}





















/*
QGroupBox *Finder::resultGroup(const QStringList &titles, const QStringList &categories)
{
    //TODO: CREAR 'resultGroup' Y 'resultLayout' EN LA CABECERA Y CAMBIAR ESTA FUNCION A bool setResultGroup Y USARLA PARA POBLAR EL LAYOUT.
    //UN INT QUE GUARDE LA CANTIDAD DE BOTONES Y LAYOUTS DE CATEGORIAS CREADOS, DESDE EL SEGUNDA LLAMADA USARLO PARA DESTRUIRLOS.
    QGroupBox *resultGroup = new QGroupBox;
    resultGroup->setStyleSheet("QGroupBox{ border:0; }");
    resultGroup->setFlat(true);

    QFont f(":/font-global");
    f.setPointSize(24); //TODO: PASAR A PORCENTAJE
    f.setCapitalization(QFont::AllUppercase);
    f.setBold(true);

    QBrush b(QColor(0, 0, 0, 255));
    QBrush b1(QColor(30, 30, 30, 255));
    QBrush b2(QColor(255, 255, 255, 255));

    QPalette p;
    p.setBrush(QPalette::Active, QPalette::Button, b);
    p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
    p.setBrush(QPalette::Active, QPalette::Text, b2);
    p.setBrush(QPalette::Active, QPalette::Base, b);
    p.setBrush(QPalette::Active, QPalette::Window, b);
    p.setBrush(QPalette::Active, QPalette::WindowText, b2);
    p.setBrush(QPalette::Active, QPalette::Highlight, b);
    p.setBrush(QPalette::Disabled, QPalette::Button, b);
    p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
    p.setBrush(QPalette::Disabled, QPalette::Text, b1);
    p.setBrush(QPalette::Disabled, QPalette::Base, b);
    p.setBrush(QPalette::Disabled, QPalette::Window, b);
    p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
    p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

    QBoxLayout *categoryLayout[categories.count()];

    for (int i = 0; i < categories.count(); i++) //FIX: BORRAR CATEGORIAS DUPLICADAS.
        categoryLayout[i] = new QBoxLayout(QBoxLayout::TopToBottom);

    QPushButton *btn[titles.count()];

    for (int i = 0; i < titles.count(); i++) {
        btn[i] = new QPushButton(titles[i], resultGroup);
        btn[i]->setFont(f);
        btn[i]->setPalette(p);
        btn[i]->setFlat(true);
        categoryLayout[i]->addWidget(btn[i]); //TODO: INCLUIR EN EL LAYOUT DE LA CATEGORIA QUE CORRESPONDE.
    }

    QGridLayout *resultLayout = new QGridLayout(resultGroup);
    for (int i = 0; i < categories.count(); i++)
        resultLayout->addLayout(categoryLayout[i], 1, 0);

    resultGroup->setLayout(resultLayout); //TODO: CREAR UNA SOLA VEZ GRUPO-LAYOUTS Y ACTUALIZARLO.

    return resultGroup;
}


QGroupBox *Finder::filterGroup(const QString &filter, const QStringList &filterData)
{
    QString icon_path;

    if (filter == "Categories")
        icon_path = ":/img-cat-";
    else
        icon_path = ":/img-star-";

    QDesktopWidget desk; //TODO: MOVERLO AL CONSTRUCTOR DE PIG.

    QGroupBox *filterGroup = new QGroupBox;
    filterGroup->setStyleSheet("QGroupBox{ border:0; }");
    filterGroup->setFlat(true);

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

    QPushButton *btn[filterData.count()];
    QGridLayout *filterLayout = new QGridLayout(filterGroup);

    //QScrollArea *area = new QScrollArea();
    //area->setWidget(filterGroup);

    int i = 0;
    for(int row = 0; row < 7; row++) {
        for(int column = 0; column < 4; column++) { //TODO: COMPROBAR QUE NO LEA UN ELEMENTO DEL ARRAY QUE NO EXISTE.
            btn[i] = new QPushButton(filterGroup);
            btn[i]->setFont(f);
            btn[i]->setPalette(p1);
            btn[i]->setFlat(true);
            btn[i]->setIcon(QIcon(icon_path+filterData[i]));
            btn[i]->setIconSize(QSize((desk.width()/5), (desk.height()/5)));
            filterLayout->addWidget(btn[i], row, column);
            i++;
        }
    }
    filterGroup->setLayout(filterLayout);

    return filterGroup;
}
*/


