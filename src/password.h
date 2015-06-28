#ifndef PASSWORD_H
#define PASSWORD_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>
#include <QString>

#include <QGroupBox>

class Password : public QObject
{
    Q_OBJECT

public:
    explicit Password(const QString *path = NULL, const bool set = NULL, QObject *parent = 0);
    ~Password();

    QGroupBox *group;

public slots:
    void check();

signals:
    void ready();
    void finished();

private:
    QFile file;
    QString digest;

    bool _set;

private slots:
    bool isMatch(const QString *plain);
    bool isWritten(const QString *plain);
    const QString calculate(const QString **plain);
    void setupUi();
};

#endif
