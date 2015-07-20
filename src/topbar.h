#ifndef TOPBAR_H
#define TOPBAR_H

#include "finder.h"
#include "ui.h"

#include <QWidget>
#include <QtSql>

namespace Ui {
class TopBar;
}

class TopBar : public QWidget
{
    Q_OBJECT

public:
    explicit TopBar(QSqlDatabase *db, QWidget *parent = 0);
    ~TopBar();

    Finder *getObject() { return finder; }

private:
    Finder *finder;

    Ui::TopBar *ui;
};

#endif

