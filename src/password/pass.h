#ifndef PASS_H
#define PASS_H

#include <QCryptographicHash>
#include <QObject>

class Pass : public QObject
{
    Q_OBJECT
public:
    explicit Pass(QObject *parent = 0);
    ~Pass();

    bool requirePassCheck();
    bool writePass(QString plain);
    bool checkPass(QString plain);

private slots:
    QString hashCalculation(QString plain);
};

#endif
