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
    explicit Finder(QSqlDatabase *db, QWidget *parent = 0);
    ~Finder();

    void setFilterOnCovers() { mFilterOnCovers = false; }

signals:
    void sendData(const QStringList *data, const QString *filter = NULL);

private:
    QSqlDatabase *_db;

    QStringList data;
    QStringList filter;

    Ui::Finder *ui;

    bool mFilterOnCovers = false;

private slots:
    QStringList *query(const QString &str = NULL, const QString &category = NULL,
                       const bool &getData = false, const bool &getFilter = false);
    void filter_handler();
};

#endif
