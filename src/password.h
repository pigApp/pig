#ifndef PASSWORD_H
#define PASSWORD_H

#include <QObject>
#include <QCryptographicHash>
#include <QString>

class Password : public QObject
{
    Q_OBJECT

public:
    explicit Password(QObject *parent = 0);
    ~Password();

    bool require();
    bool write(const QString *plain);
    bool success(const QString *plain);

private:
    QString pd;

private slots:
    const QString calculate(const QString **plain);
};

#endif
