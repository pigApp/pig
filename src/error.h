#ifndef ERROR_H
#define ERROR_H

#include "ui.h"

#include <QWidget>

class Error : public QWidget
{
    Q_OBJECT

public:
    explicit Error(const QString *errorMsg, QWidget *parent = 0);
    ~Error();

private:
    Ui::Error *ui;
};

#endif
