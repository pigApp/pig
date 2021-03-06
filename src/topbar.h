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

    Finder *get_finder_obj() { return m_finder; }
    QPushButton *get_buttonSetup_obj() { return m_b_setup; }

private:
    Finder *m_finder;
    QPushButton *m_b_setup;

    Ui::TopBar *ui;
};

#endif
