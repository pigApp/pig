#include "setup.h"

Setup::Setup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);
}

Setup::~Setup()
{
    delete ui;
}
