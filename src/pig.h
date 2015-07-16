#ifndef PIG_H
#define PIG_H

#include "topbar.h"
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
    View *view;

    Ui::PIG *ui;

private slots:
    void authorization(bool set);
    void update();
    void viewer(const QStringList *data);
    void widgetsHandler(QWidget *w, bool add);
};

#endif
