#include "topbar.h"

TopBar::TopBar(QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopBar)
{
    ui->setupUi(this);

    m_button_setup = ui->button_setup;

    m_finder = new Finder(db, ui->layout, this);
}

TopBar::~TopBar()
{
    delete ui;
}

