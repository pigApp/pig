#include "topbar.h"

TopBar::TopBar(QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopBar)
{
    ui->setupUi(this);

    m_b_setup = ui->b_setup;

    m_finder = new Finder(db, ui->l, this);
}

TopBar::~TopBar()
{
    delete ui;
}

