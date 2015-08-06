#ifndef PIG_H
#define PIG_H

#include "topbar.h"
#include "setup.h"
#include "view.h"
#include "ui.h"

#include <QWidget>

namespace Ui {
class PIG;
}

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent = 0);
    ~PIG();

private:
    QString PIG_PATH;
    QSqlDatabase db;

    TopBar *topbar;
    Setup *setup;
    View *view;

    Ui::PIG *ui;

private slots:
    void init_authorization();
    void init_update();
    void init_topbar();
    void init_viewer(const QStringList *data, const QString *filter);
    void init_setup();
    void error(QString error);
};

#endif
