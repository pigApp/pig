#include "finder.h"

#include <QTimer>
#include <QDebug>//

Finder::Finder(QSqlDatabase *db, QGridLayout *l_topbar, QWidget *parent) :
    QWidget(parent),
    _db(db),
    isFiltersHidden(true),
    quality("ALL"),
    fullMovie("ALL"),
    ui(new Ui::Finder)
{
    ui->setupUi(query("", NULL, true), l_topbar, this);

    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        data.clear();
        emit sendData(query((ui->input->selectAll(), ui->input->selectedText()), NULL, false, true));
        ui->input->deselect();
    });
    connect (ui->b_filters, SIGNAL(pressed()), this, SLOT(filters_handler()));

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
    if (isFiltersHidden)
        ui->b_filters->setIcon(QIcon(":/icon-filters-dark"));
    else
        ui->b_filters->setIcon(QIcon(":/icon-filters"));

    if (ui->cb_categories == 0) {
        ui->setupFilterUi(query("categories", NULL, false, false, true), this);

        QObject::connect (ui->cb_categories, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (!m_filterOnCovers) {
                data.clear();
                emit sendData(query(NULL, ui->cb_categories->currentText(), false, true, false));
                ui->input->deselect();
            } else {
                qDebug() << "test";
                //FIX: DIRECCION DE CURRENT INDEX ES TEMPORAL.
                //emit sendData(NULL, &ui->cb_quality->currentText());
            }
        });

        QObject::connect (ui->cb_pornstars, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            if (!m_filterOnCovers) {
                data.clear();
                emit sendData(query(NULL, ui->cb_pornstars->currentText(), false, true, false));
                ui->input->deselect();
            } else {
                qDebug() << "test";
                //FIX: DIRECCION DE CURRENT INDEX ES TEMPORAL.
                //emit sendData(NULL, &ui->cb_quality->currentText());
            }
        });

        QObject::connect (ui->cb_quality, static_cast<void (QComboBox::*)(const QString &)>
                          (&QComboBox::currentIndexChanged), [=] {
            quality = ui->cb_quality->currentText();
            if (m_filterOnCovers)
                emit sendData(NULL, &quality);
        });

        QObject::connect (ui->b_fullMovie, &QPushButton::pressed, [=] {
            if (!ui->b_fullMovie->isChecked()) {
                fullMovie = "FULL";
                ui->b_fullMovie->setPalette(ui->p_filters_active);
            } else {
                fullMovie = "ALL";
                ui->b_fullMovie->setPalette(ui->p_filters);
            }

            if (m_filterOnCovers)
                emit sendData(NULL, &fullMovie);
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

void Finder::test()
{
    emit sendData(query("", NULL, false, true));
}
