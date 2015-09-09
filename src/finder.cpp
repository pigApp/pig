#include "finder.h"

#include <QTimer>
#include <QDebug>//

Finder::Finder(QSqlDatabase *db, QGridLayout *l_topbar, QWidget *parent) :
    QWidget(parent),
    _db(db),
    isFiltersHidden(true),
    isFilteringCovers(true),
    filtersReset(false),
    ui(new Ui::Finder)
{
    ui->setupUi(query(NULL, NULL, NULL, NULL, NULL, true), l_topbar, this);

    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        data.clear();
        emit sendData(query((ui->input->selectAll(), ui->input->selectedText()),
                            NULL, NULL, NULL, NULL, false, true));
        ui->input->deselect();
    });
    connect (ui->b_filters, SIGNAL(pressed()), this, SLOT(filters()));

    QTimer *t = new QTimer(this);
    QObject::connect (t, &QTimer::timeout, [&] {
        emit sendData(query(NULL, NULL, NULL, NULL, NULL, false, true, false,
                            "date(timestamp) DESC LIMIT 1000"));
    });
    t->setSingleShot(true);
    t->start(100);
}

Finder::~Finder()
{
    delete ui;
}

QStringList *Finder::query(const QString &name, const QString &pornstar, const QString &quality,
                           const QString &full, const QString &category, const bool &getMovies,
                           const bool &getData, const bool &getFilter, const QString &order)
{
    if (_db->open()) {
        QSqlQuery query;

        if (getMovies) {
            query.prepare("SELECT name FROM Movies ORDER BY name ASC LIMIT 1000");
        } else if (getData) {
            query.prepare("SELECT name, date, cas, quality, time, full, category, timestamp \
                          , hostCovers, urlCover, urlBackCover, hostPreview, urlPreview \
                          , hostTorrent, urlTorrent, scenes FROM Movies WHERE name LIKE \
                          '"+name+"%' AND cas LIKE '%"+pornstar+"%' AND quality LIKE \
                          '%"+quality+"%' AND full LIKE '%"+full+"%' AND category LIKE \
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

void Finder::filters()
{
    if (isFiltersHidden)
        ui->b_filters->setIcon(QIcon(":/icon-filters-dark"));
    else
        ui->b_filters->setIcon(QIcon(":/icon-filters"));

    if (ui->cb_categories == 0) {
        ui->setupFilterUi(query("categories", NULL, NULL, NULL, NULL, false, false, true),
                          query("pornstars", NULL, NULL, NULL, NULL, false, false, true), this);

        QObject::connect (ui->cb_categories, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (filtersReset)
                ui->cb_categories->setStyleSheet(ui->cb_style);
            else
                filter_handler(&ui->cb_categories, "CATEGORY");
        });
        QObject::connect (ui->cb_pornstars, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (filtersReset)
                ui->cb_pornstars->setStyleSheet(ui->cb_style);
            else
                filter_handler(&ui->cb_pornstars, "PORNSTAR");
        });
        QObject::connect (ui->cb_quality, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (filtersReset)
                ui->cb_quality->setStyleSheet(ui->cb_style);
            else
                filter_handler(&ui->cb_quality, "QUALITY");
        });
        QObject::connect (ui->b_fullMovie, &QPushButton::pressed, [=] {
            filter_handler(NULL, "FULLMOVIE");
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

void Finder::filter_handler(QComboBox **cb, const QString &item)
{
    //TODO: REVISAR b_fullMovie setChecked.
    //TODO: INPUT FOCUS.
    //TODO: BOTON 'fitrar solo resultados'.

    filtersReset = true;

    filter.clear();

    if (cb == 0) {
        ui->cb_categories->setCurrentIndex(0);
        ui->cb_pornstars->setCurrentIndex(0);
        ui->cb_quality->setCurrentIndex(0);

        if (ui->b_fullMovie->isChecked()) {
            if (isFilteringCovers) {
                filter << item << NULL;
                emit sendData(NULL, &filter);
            } else {
                data.clear();
                emit sendData(query(NULL, NULL, NULL, NULL, NULL, false, true));
            }

            ui->b_fullMovie->setChecked(false);//
            ui->b_fullMovie->setPalette(ui->p_fullMovie);
        } else {
            if (isFilteringCovers) {
                filter << item << "YES";
                emit sendData(NULL, &filter);
            } else {
                data.clear();
                emit sendData(query(NULL, NULL, NULL, "YES", NULL, false, true));
            }

            ui->b_fullMovie->setChecked(true);//
            ui->b_fullMovie->setPalette(ui->p_fullMovie_active);
        }
    } else {
        if ((*cb) == ui->cb_categories) {
            ui->cb_pornstars->setCurrentIndex(0);
            ui->cb_quality->setCurrentIndex(0);
        } else if ((*cb) == ui->cb_pornstars) {
            ui->cb_categories->setCurrentIndex(0);
            ui->cb_quality->setCurrentIndex(0);
        } else if ((*cb) == ui->cb_quality) {
            ui->cb_categories->setCurrentIndex(0);
            ui->cb_pornstars->setCurrentIndex(0);
        }

        ui->b_fullMovie->setChecked(false);//
        ui->b_fullMovie->setPalette(ui->p_fullMovie);

        if ((*cb)->currentText() == item) {
            if (isFilteringCovers) {
                filter << item << NULL;
                emit sendData(NULL, &filter);
            } else {
                data.clear();
                emit sendData(query(NULL, NULL, NULL, NULL, NULL, false, true));
            }

            (*cb)->setStyleSheet(ui->cb_style);
        } else {
            if (isFilteringCovers) {
                filter << item << (*cb)->currentText();
                emit sendData(NULL, &filter);
            } else {
                data.clear();
                if ((*cb) == ui->cb_categories)
                    emit sendData(query(NULL, NULL, NULL, NULL, ui->cb_categories->currentText(),
                                        false, true));
                else if ((*cb) == ui->cb_pornstars)
                    emit sendData(query(NULL, ui->cb_pornstars->currentText(), NULL, NULL, NULL,
                                        false, true));
                else if ((*cb) == ui->cb_quality)
                    emit sendData(query(NULL, NULL, ui->cb_quality->currentText(),NULL, NULL,
                                        false, true));
            }

            (*cb)->setStyleSheet(ui->cb_style_active);
        }
    }

    filtersReset = false;
}

void Finder::setFinderState(const bool &block)
{
    if (block) {
        if (ui->b_filters != 0)
            ui->b_filters->setIcon(QIcon(":/icon-filters-dark"));
    } else {
        if (ui->b_filters != 0)
            ui->b_filters->setIcon(QIcon(":/icon-filters"));
    }
    
    ui->input->setDisabled(block);

    if (ui->cb_categories != 0) {
        ui->cb_categories->setDisabled(block);
        ui->cb_pornstars->setDisabled(block);
        ui->cb_quality->setDisabled(block);
        ui->b_fullMovie->setDisabled(block);
    }
}
