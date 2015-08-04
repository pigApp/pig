#ifndef SU_H
#define SU_H

#include <QObject>
#include <QProcess>

class Su : public QObject
{
    Q_OBJECT

public:
    explicit Su(QObject *parent = 0);
    ~Su();

signals:
    void finished(int exitCode);

public slots:
    void install(const QString arg);
    void authorization(const QString arg);

private:
    QString manager;
    QProcess *p;
};

#endif
