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

    Finder *getFinderObject() { return m_finder; }
    QPushButton *getBtnSetupObject() { return m_button_setup;}

private:
    Finder *m_finder;

    Ui::TopBar *ui;
    QPushButton *m_button_setup;
};

#endif
