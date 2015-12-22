#ifndef FINDER_H
#define FINDER_H

#include "ui.h"

#include <QWidget>

#include <QtSql>

namespace Ui {
class Finder;
}

class Finder : public QWidget
{
    Q_OBJECT

public:
    explicit Finder(QSqlDatabase *db, QGridLayout *l_topbar, QWidget *parent = 0);
    ~Finder();

signals:
    void sendData(const QStringList *data, const QStringList *filter = NULL);
    void sendError(QString errorMsg);

private:
    QSqlDatabase *_db;
    
    bool isFiltersHidden;
    bool isFilteringResult;
    bool filtersReset;

    Ui::Finder *ui;

    QStringList movies;
    QStringList data;
    QStringList filter;
    QStringList categories;
    QStringList pornstars;

private slots:
    QStringList *query(const QString &name = NULL, const QString &pornstar = NULL,
                       const QString &quality = NULL, const QString &full = NULL,
                       const QString &category = NULL, const bool &getMovies = false,
                       const bool &getData = false, const bool &getFilter = false,
                       const QString &order = "name ASC LIMIT 1000");
    void filters();
    void filters_handler(QComboBox **cb = NULL, const QString &item = NULL);
};

#endif
