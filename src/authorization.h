#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>
#include <QGroupBox>

class Auth : public QObject
{
    Q_OBJECT

public:
    explicit Auth(const QString *PIG_PATH, bool set, QObject *parent = 0);
    ~Auth();

public slots:
    void check();

signals:
    void sendGroup(QGroupBox *group = NULL, bool add = false);
    void finished();

private:
    QFile file;
    QString digest;
    QGroupBox *group;

    bool _set;

private slots:
    void match(const QString &str);
    void set(const QString &str);
    void reset();
    const QString calculate(const QString *plain);
    void setup_ui();
};

#endif
