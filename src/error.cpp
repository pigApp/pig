#include "error.h"

Error::Error(const QString *errorMsg, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Error)
{
    ui->setupUi(&errorMsg, this);
    QObject::connect (ui->b, &QPushButton::pressed, [=] { exit(0); });
}

Error::~Error()
{
    delete ui;
}
