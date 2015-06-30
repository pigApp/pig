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

    inline QGroupBox *getGroup() { return group; }

public slots:
    void check();

signals:
    void ready();
    void finished();

private:
    QFile file;
    QString digest;
    QGroupBox *group;

    bool _set;

private slots:
    void set(const QString &str);
    void reset();
    void match(const QString &str);
    const QString calculate(const QString *plain);
    void setup_ui();
};

#endif
