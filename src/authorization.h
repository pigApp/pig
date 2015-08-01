#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include "ui.h"

#include <QWidget>

#include <QFile>

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(const QString *PIG_PATH, bool _set, QWidget *parent = 0);
    ~Authorization();

signals:
    void showWidget(QWidget *w);

public slots:
    void check();

private:
    bool __set;

    QFile file;
    QString digest;

    Ui::Authorization *ui;

private slots:
    void set(const QString &str);
    void reset();
    void match(const QString &str);
    const QString calculate(const QString *plain);
    void init_ui();
};

#endif
