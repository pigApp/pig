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
    void sendData(const QStringList *data, const QString *filter = NULL);

private:
    QSqlDatabase *_db;

    QStringList data;

    Ui::Finder *ui;

    QString filter;//

private slots:
    void query(const QString &str = NULL, bool getData = false, bool getFilter = false);
};

#endif
