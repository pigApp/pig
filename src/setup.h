#ifndef SETUP_H
#define SETUP_H

#include "authorization.h"//
#include "ui.h"

#include <QWidget>

#include <QtSql>

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT

public:
    explicit Setup(const QString *PIG_PATH, QSqlDatabase *db, QWidget *parent = 0);
    ~Setup();

signals:

private:
    const QString *_PIG_PATH;
    QSqlDatabase *_db;

    Ui::Setup *ui;
};

#endif
