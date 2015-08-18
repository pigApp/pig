#ifndef TOPBAR_H
#define TOPBAR_H

#include "finder.h"
#include "ui.h"

#include <QWidget>
#include <QPushButton>
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

    Finder *getFinderObj() { return m_finder; }
    QPushButton *getButtonSetupObj() { return m_b_setup; }

private:
    Finder *m_finder;

    Ui::TopBar *ui;

    QPushButton *m_b_setup;
};

#endif
