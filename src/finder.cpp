#include "finder.h"

#include <QTimer>
#include <QDebug>//

Finder::Finder(QSqlDatabase *db, QGridLayout *l_topbar, QWidget *parent) :
    QWidget(parent),
    _db(db),
    isFiltersHidden(true),
    ui(new Ui::Finder)
{
    ui->setupUi(query(NULL, NULL, NULL, true), l_topbar, this);

    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        data.clear();
        emit sendData(query((ui->input->selectAll(), ui->input->selectedText()),
                      NULL, NULL, false, true));
        ui->input->deselect();
    });
    connect (ui->b_filters, SIGNAL(pressed()), this, SLOT(filters_handler()));

    QTimer *t = new QTimer(this);
    QObject::connect (t, &QTimer::timeout, [&] {
        emit sendData(query(NULL, NULL, NULL, false, true, false, "date(timestamp) DESC LIMIT 1000"));
    });
    t->setSingleShot(true);
    t->start(100);
}

Finder::~Finder()
{
    delete ui;
}

QStringList *Finder::query(const QString &name, const QString &pornstar, const QString &category,
                           const bool &getMovies, const bool &getData, const bool &getFilter,
                           const QString &order)
{
    if (_db->open()) {
        QSqlQuery query;

        if (getMovies) {
            query.prepare("SELECT name FROM Movies ORDER BY name ASC LIMIT 1000");
        } else if (getData) {
            query.prepare("SELECT name, date, cas, quality, time, full, category, timestamp \
                          , hostCovers, urlCover, urlBackCover, hostPreview, urlPreview \
                          , hostTorrent, urlTorrent, scenes FROM Movies WHERE name LIKE \
                          '"+name+"%' AND cas LIKE '%"+pornstar+"%' AND category LIKE \
                          '%"+category+"%' ORDER BY "+order+"");
        } else if (getFilter) {
            query.prepare("SELECT "+name+" FROM filters");
        }

        if (!query.exec()) {
            (_db)->close();
            //db_error();
        } else {
            for (int i = 0; query.next(); i++) {
                if (getMovies) { 
                    movies << query.value(0).toString().split(",");
                } else if (getData) {
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
                    data.append(query.value(0).toString()+"_front.jpg");
                    data.append(query.value(10).toString());
                    data.append(query.value(0).toString()+"_back.jpg");
                    data.append(query.value(11).toString());
                    data.append(query.value(12).toString());
                    data.append(query.value(0).toString()+".mp4");
                    data.append(query.value(13).toString());
                    data.append(query.value(14).toString());
                    data.append(query.value(15).toString());
                } else if (getFilter) {
                    if (name == "categories")
                        categories.append(query.value(0).toString().split(","));
                    else
                        pornstars.append(query.value(0).toString().split(","));
                }
            }
            (_db)->close();

            if (getMovies) {
                return &movies;
            } else if (getData && !data.isEmpty()) {
                m_filterOnCovers = true;
                return &data;
            } else if (getFilter) {
                if (name == "categories")
                    return &categories;
                else
                    return &pornstars;
            }
        }
    } else {
        //db_error();
    }
    return 0;
}

void Finder::filters_handler()
{
    if (isFiltersHidden)
        ui->b_filters->setIcon(QIcon(":/icon-filters-dark"));
    else
        ui->b_filters->setIcon(QIcon(":/icon-filters"));

    if (ui->cb_categories == 0) {
        ui->setupFilterUi(query("categories", NULL, NULL, false, false, true),
                          query("pornstars", NULL, NULL, false, false, true), this);

        QObject::connect (ui->cb_categories, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (!m_filterOnCovers) {
                data.clear();
                if (ui->cb_categories->currentText() == "CATEGORY")
                    emit sendData(query(NULL, NULL, NULL, false, true));
                else
                    emit sendData(query(NULL, NULL, ui->cb_categories->currentText(), false, true));
                ui->input->deselect();
            } else {
                filter.clear();
                if (ui->cb_categories->currentText() == "CATEGORY")
                    filter << "CATEGORY" << NULL;
                else
                    filter << "CATEGORY" << ui->cb_categories->currentText();
                emit sendData(NULL, &filter);
            }
        });
        QObject::connect (ui->cb_pornstars, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (!m_filterOnCovers) {
                data.clear();
                if (ui->cb_pornstars->currentText() == "PORNSTAR")
                    emit sendData(query(NULL, NULL, NULL, false, true));
                else
                    emit sendData(query(NULL, ui->cb_pornstars->currentText(), NULL, false, true));
                ui->input->deselect();
            } else {
                filter.clear();
                if (ui->cb_pornstars->currentText() == "PORNSTAR")
                    filter << "PORNSTAR" << NULL;
                else
                    filter << "PORNSTAR" << ui->cb_pornstars->currentText();
                emit sendData(NULL, &filter);
            }
        });
        QObject::connect (ui->cb_quality, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (m_filterOnCovers) {
                filter.clear();
                if (ui->cb_quality->currentText() == "QUALITY")
                    filter << "QUALITY" << NULL;
                else
                    filter << "QUALITY" << ui->cb_quality->currentText();
                emit sendData(NULL, &filter);
            }
        });
        QObject::connect (ui->b_fullMovie, &QPushButton::pressed, [=] {
            if (!ui->b_fullMovie->isChecked()) {
                ui->b_fullMovie->setPalette(ui->p_filters_active);
                if (m_filterOnCovers) { // POSIBLEMENTE NO ES NECESARIO
                    filter.clear();
                    filter << "FULLMOVIE" << "YES";
                    emit sendData(NULL, &filter);
                }
            } else {
                ui->b_fullMovie->setPalette(ui->p_filters);
                if (m_filterOnCovers) { // POSIBLEMENTE NO ES NECESARIO
                    filter.clear();
                    filter << "FULLMOVIE" << NULL;
                    emit sendData(NULL, &filter);
                }
            }
        });
        
        isFiltersHidden = false;
    } else {
        if (isFiltersHidden)
            ui->_l_topbar->addItem(ui->l_filters, 1, 0);
        else
            ui->_l_topbar->removeItem(ui->l_filters);

        isFiltersHidden = !isFiltersHidden;
    }
}
