#include "topbar.h"

TopBar::TopBar(QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopBar)
{
    ui->setupUi(this);

    finder = new Finder(db, this);
    ui->layout->addWidget(finder);
}

TopBar::~TopBar()
{
    delete ui;
}

