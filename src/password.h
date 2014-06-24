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

    bool requirePassword();
    bool writePassword(QString plain);
    bool rightPassword(QString plain);

private slots:
    QString hashCalculation(QString plain);
};

#endif
