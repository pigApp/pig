#ifndef PASSWORD_H
#define PASSWORD_H

#include <QObject>
#include <QCryptographicHash>

class Password : public QObject
{
    Q_OBJECT

public:
    explicit Password(QObject *parent = 0);
    ~Password();

    bool require();
    bool write(QString plain);
    bool right(QString plain);

private slots:
    QString calculate(QString plain);
};

#endif
