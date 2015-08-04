#include "finder.h"

#include <QTimer>
#include <QDebug>//

Finder::Finder(QSqlDatabase *db, QGridLayout *layout_topbar, QWidget *parent) :
    QWidget(parent),
    _db(db),
    quality("ALL"),
    fullMovie("ALL"),
    ui(new Ui::Finder)
{
    ui->setupUi(query("", NULL, true), layout_topbar, this);

    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        data.clear();
        emit sendData(query((ui->input->selectAll(), ui->input->selectedText()), NULL, false, true));
        ui->input->deselect();
    });
    connect (ui->button_filters, SIGNAL(pressed()), this, SLOT(filters_handler()));

    //if (init)
        QTimer::singleShot(500, this, SLOT(test()));
}

Finder::~Finder()
{
    delete ui;
}

QStringList *Finder::query(const QString &str, const QString &category, const bool &getList,
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
            query.prepare("SELECT title, cas, category, quality, time, full \
                          , hostCovers, urlCover, urlBackCover, hostPreview, urlPreview \
                          , hostTorrent, urlTorrent, scenes FROM Movies WHERE title LIKE \
                          '"+str+"%' AND cas LIKE '%"+pornstar+"%' AND category LIKE \
                          '%"+category+"%' AND quality LIKE '%"+_quality+"%' AND full LIKE \
                          '%"+_fullMovie+"%' ORDER BY title ASC LIMIT 1000");
        } else if (getFilter) {
            query.prepare("SELECT "+str+" FROM filters");
        } else if (getList){
            query.prepare("SELECT title FROM Movies WHERE title LIKE \
                          '"+str+"%' AND cas LIKE '%"+pornstar+"%' AND category LIKE \
                          '%"+category+"%' AND quality LIKE '%"+_quality+"%' AND full LIKE \
                          '%"+_fullMovie+"%' ORDER BY title ASC LIMIT 1000");
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
                    data.append(query.value(0).toString()+"_front.jpg");
                    data.append(query.value(8).toString());
                    data.append(query.value(0).toString()+"_back.jpg");
                    data.append(query.value(9).toString());
                    data.append(query.value(10).toString());
                    data.append(query.value(0).toString()+".mp4");
                    data.append(query.value(11).toString());
                    data.append(query.value(12).toString());
                    data.append(query.value(13).toString());
                } else if (getFilter) {
                    filter << query.value(0).toString().split(",");
                } else if (getList) {
                    movies << query.value(0).toString().split(",");
                }
            }

            (_db)->close();

            if (getData && !data.isEmpty()) {
                m_filterOnCovers = true;
                return &data;
            } else if (getFilter) {
                return &filter;
            } else if (getList) {
                return &movies;
            }
        }
    } else {
        //db_error();
    }

    return 0;
}


void Finder::filters_handler()
{
    if (ui->buttons_categories_vector.isEmpty()) {
        const QStringList *categories = query("categories", NULL, false, false, true);

        ui->setupFilterUi(this);

        QObject::connect (ui->radio_allp, &QRadioButton::released, [=] {
            quality = "ALL";
            if (m_filterOnCovers)
                emit sendData(NULL, &quality);
        });
        QObject::connect (ui->radio_720p, &QRadioButton::released, [=] {
            quality = "720p";
            if (m_filterOnCovers)
                emit sendData(NULL, &quality);
        });
        QObject::connect (ui->radio_1080p, &QRadioButton::released, [=] {
            quality = "1080p";
            if (m_filterOnCovers)
                emit sendData(NULL, &quality);
        });

        QObject::connect (ui->radio_fullMovie, &QRadioButton::released, [=] {
            if (ui->radio_fullMovie->isChecked())
                fullMovie = "FULL";
            else
                fullMovie = "ALL";

            if (m_filterOnCovers)
                emit sendData(NULL, &fullMovie);
        });

        QFont font(":/font-global");
        font.setPointSize(12); //TODO: PORCENTAJE
        font.setCapitalization(QFont::AllUppercase);
        font.setBold(true);

        for (int i = 0; i < (*categories).size(); i++) {
            ui->buttons_categories_vector.push_back(new QPushButton((*categories)[i], this));
            ui->buttons_categories_vector.last()->setFont(font);
            ui->buttons_categories_vector.last()->setFlat(true);

            QObject::connect (ui->buttons_categories_vector.last(), &QPushButton::pressed, [=] {
                if (!m_filterOnCovers) {
                    data.clear();
                    emit sendData(query(NULL, (*categories)[i], false, true, false));
                    ui->input->deselect();
                } else {
                    emit sendData(NULL, &(*categories)[i]);
                }
            });

            ui->layout_filters->addWidget(ui->buttons_categories_vector.last());
        }
    } else {
        if (isFiltersHidden)
            ui->__layout_topbar->addItem(ui->layout_filters, 1, 0);
        else
            ui->__layout_topbar->removeItem(ui->layout_filters);

        isFiltersHidden = !isFiltersHidden;
    }
}

void Finder::test()
{
    emit sendData(query("", NULL, false, true));
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

    QPushButton *button[titles.count()];

    for (int i = 0; i < titles.count(); i++) {
        button[i] = new QPushButton(titles[i], resultGroup);
        button[i]->setFont(f);
        button[i]->setPalette(p);
        button[i]->setFlat(true);
        categoryLayout[i]->addWidget(button[i]); //TODO: INCLUIR EN EL LAYOUT DE LA CATEGORIA QUE CORRESPONDE.
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

    QPushButton *button[filterData.count()];
    QGridLayout *filterLayout = new QGridLayout(filterGroup);

    //QScrollArea *area = new QScrollArea();
    //area->setWidget(filterGroup);

    int i = 0;
    for(int row = 0; row < 7; row++) {
        for(int column = 0; column < 4; column++) { //TODO: COMPROBAR QUE NO LEA UN ELEMENTO DEL ARRAY QUE NO EXISTE.
            button[i] = new QPushButton(filterGroup);
            button[i]->setFont(f);
            button[i]->setPalette(p1);
            button[i]->setFlat(true);
            button[i]->setIcon(QIcon(icon_path+filterData[i]));
            button[i]->setIconSize(QSize((desk.width()/5), (desk.height()/5)));
            filterLayout->addWidget(button[i], row, column);
            i++;
        }
    }
    filterGroup->setLayout(filterLayout);

    return filterGroup;
}
*/


