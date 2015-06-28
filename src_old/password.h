#ifndef PASSWORD_H
#define PASSWORD_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>

class Password : public QObject
{
    Q_OBJECT

public:
    explicit Password(QObject *parent = 0);
    ~Password();

public slots:
    bool check(const QString *plain);
    bool write(const QString *plain);

private:
    QFile file;

private slots:
    const QString calculate(const QString **plain);
};

#endif
