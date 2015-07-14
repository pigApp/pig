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

signals:
    void sendData(const QStringList &data);
    //void sendGroup(QGroupBox *group = NULL, bool add = false);

private:
    QSqlDatabase *_db;
    Ui::Finder *ui;

private slots:
    void query(const QString &str = NULL, bool getData = false, bool getFilter = false);
};

#endif


/*
    QGroupBox *resultGroup(const QStringList &titles, const QStringList &categories);
    QGroupBox *filterGroup(const QString &filter, const QStringList &filterData);
*/
