#ifndef SETUP_H
#define SETUP_H

#include "ui.h"

#include <QWidget>

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT

public:
    explicit Setup(QWidget *parent = 0);
    ~Setup();

signals:

public slots:

private:
    Ui::Setup *ui;
};

#endif
