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

    void setFilterOnCovers() { m_filterOnCovers = false; }

signals:
    void sendData(const QStringList *data, const QString *filter = NULL);

private:
    QSqlDatabase *_db;

    QStringList data;
    QStringList filter;
    QStringList movies;

    bool isFiltersHidden;

    QString quality;
    QString fullMovie;

    Ui::Finder *ui;

    bool m_filterOnCovers = false;

private slots:
    QStringList *query(const QString &name = NULL, const QString &category = NULL,
                       const bool &getList = false, const bool &getData = false,
                       const bool &getFilter = false, const QString &order = "name ASC LIMIT 1000");
    void filters_handler();
};

#endif
