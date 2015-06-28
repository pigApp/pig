#ifndef PIG_H
#define PIG_H

#include "topbar.h"
#include "password.h"

#include <QString>
#include <QStringList>

#include <QWidget>
#include <QVBoxLayout>

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent = 0);
    ~PIG();

private:
    TopBar *pTopbar;

    QString path;
    QVBoxLayout *layout;

private slots:
    void authorization(const bool set);
    void showData(const QStringList &data);
    void setupUi();
};

#endif
