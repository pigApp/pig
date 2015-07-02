#ifndef PIG_H
#define PIG_H

#include "topbar.h"

#include <QWidget>
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
    QVBoxLayout *mainLayout;

private slots:
    void authorization(const bool set);
    void showData(const QStringList &data);
    void groupHandler(QGroupBox *group, const bool add);
    void setup_ui();
};

#endif
