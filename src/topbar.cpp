#include "topbar.h"

TopBar::TopBar(QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopBar)
{
    ui->setupUi(this);

    m_btn_setup = ui->btn_setup;

    m_finder = new Finder(db, ui->layout, this);
}

TopBar::~TopBar()
{
    delete ui;
}

