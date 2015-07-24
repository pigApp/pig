#include "finder.h"

#include <QTimer>
#include <QDebug>//

Finder::Finder(QSqlDatabase *db, QGridLayout *layoutTopbar, QWidget *parent) :
    QWidget(parent),
    _db(db),
    quality("ALL"),
    fullMovie("ALL"),
    ui(new Ui::Finder)
{
    ui->setupUi(layoutTopbar, this);

    connect (ui->btnFilters, SIGNAL(pressed()), this, SLOT(filters_handler()));
    QObject::connect (ui->input, &QLineEdit::textChanged, [&] (const QString str) { query(str); });
    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        data.clear();
        emit sendData(query((ui->input->selectAll(), ui->input->selectedText()), NULL, true));
        ui->input->deselect();
    });

    //if (init)
        QTimer::singleShot(500, this, SLOT(test()));
}

Finder::~Finder()
{
    delete ui;
}

QStringList *Finder::query(const QString &str, const QString &category,
                           const bool &getData, const bool &getFilter)
{
    const QString pornstar = "";

    QString _quality;
    if (quality == "ALL")
        _quality = "";
    else
        _quality = quality;

    QString _fullMovie;
    if (fullMovie == "ALL")
        _fullMovie = "";
    else
        _fullMovie = fullMovie;

    if (_db->open()) {
        QSqlQuery query;

        if (getData) {
            query.prepare("SELECT id, Title, Cas, Category, Quality, Time, Full \
                          , HostCover, UrlCoverFront, UrlCoverBack, HostPreview, UrlPreview \
                          , HostTorrent, UrlTorrent, Scenes FROM Movies WHERE Title LIKE \
                          '"+str+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE \
                          '%"+category+"%' AND Quality LIKE '%"+_quality+"%' AND Full LIKE \
                          '%"+_fullMovie+"%' ORDER BY Title ASC LIMIT 1000");
        } else if (getFilter) {
            query.prepare("SELECT "+str+" FROM FiltersData");
        } else {
            query.prepare("SELECT id FROM Movies WHERE Title LIKE \
                          '"+str+"%' AND Cas LIKE '%"+pornstar+"%' AND Category LIKE \
                          '%"+category+"%' AND Quality LIKE '%"+_quality+"%' AND Full LIKE \
                          '%"+_fullMovie+"%' ORDER BY Title ASC LIMIT 1");
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


void Finder::filters_handler()
{
    if (ui->btn_categories_vector.isEmpty()) {
        const QStringList *categories = query("Categories", NULL, false, true);

        ui->setupFilterUi(this);

        QObject::connect (ui->btnAllp, &QRadioButton::released, [=] {
            quality = "ALL";
            if (mFilterOnCovers)
                emit sendData(NULL, &quality);
        });
        QObject::connect (ui->btn720p, &QRadioButton::released, [=] {
            quality = "720p";
            if (mFilterOnCovers)
                emit sendData(NULL, &quality);
        });
        QObject::connect (ui->btn1080p, &QRadioButton::released, [=] {
            quality = "1080p";
            if (mFilterOnCovers)
                emit sendData(NULL, &quality);
        });

        QObject::connect (ui->btnFullMovie, &QRadioButton::released, [=] {
            if (ui->btnFullMovie->isChecked())
                fullMovie = "FULL";
            else
                fullMovie = "ALL";

            if (mFilterOnCovers)
                emit sendData(NULL, &fullMovie);
        });

        QFont f(":/font-global");
        f.setPointSize(12); //TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Text, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        for (int i = 0; i < (*categories).size(); i++) {
            ui->btn_categories_vector.push_back(new QPushButton((*categories)[i], this));
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

            ui->layoutFilters->addWidget(ui->btn_categories_vector.last());
        }
    } else {
        if (isFiltersHidden)
            ui->__layoutTopbar->addItem(ui->layoutFilters, 1, 0);
        else
            ui->__layoutTopbar->removeItem(ui->layoutFilters);

        isFiltersHidden = !isFiltersHidden;
    }
}

void Finder::test()
{
    emit sendData(query("", NULL, true));
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


