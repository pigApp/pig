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
    explicit Authorization(const QString* const PIG_PATH, bool set_, QWidget *parent = 0);
    ~Authorization();

signals:
    void sendWidget(QWidget *w);

public slots:
    void check();

private:
    const QString* const _PIG_PATH;
    bool _set;

    bool isSet;

    Ui::Authorization *ui;

    QFile file;
    QString digest;

private slots:
    void set(const QString &str);
    void reset();
    void match(const QString &str);
    const QString calculate(const QString *plain);
    void set_icon(const bool &isReset, const bool &hasFailed = false);
    void init_ui();
};

#endif
