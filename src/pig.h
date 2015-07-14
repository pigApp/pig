#ifndef PIG_H
#define PIG_H

#include "topbar.h"

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent = 0);
    ~PIG();

private:
    TopBar *topbar;

    QString PIG_PATH;
    QSqlDatabase db;
    QVBoxLayout *mainLayout;

    QStringList test;//

private slots:
    void authorization(bool set);
    void update();
    //void viewer(const QStringList &data);
    void viewer();
    void groupHandler(QGroupBox *group, bool add);
    void setup_ui();
};

#endif
