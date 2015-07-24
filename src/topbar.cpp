#include "topbar.h"

TopBar::TopBar(QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopBar)
{
    ui->setupUi(this);

    finder = new Finder(db, ui->layout, this);

    //new setup
    ui->layout->addWidget(ui->btnSetup, 0, 1);

}

TopBar::~TopBar()
{
    delete ui;
}

