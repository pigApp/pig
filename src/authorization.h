#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include "ui.h"

#include <QWidget>

#include <QFile>

namespace Ui {
class Auth;
}

class Auth : public QWidget
{
    Q_OBJECT

public:
    explicit Auth(const QString *PIG_PATH, bool set_, QWidget *parent = 0);
    ~Auth();

signals:
    void sendWidget(QWidget *w = NULL, bool add = false);

public slots:
    void check();

private:
    bool _set;

    QFile file;
    QString digest;

    Ui::Auth *ui;

private slots:
    void set(const QString &str);
    void reset();
    void match(const QString &str);
    const QString calculate(const QString *plain);
    void initUi();
};

#endif
