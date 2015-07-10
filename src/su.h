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

public slots:
    void update(const QString arg);
    void auth(const QString arg);

signals:
    void finished(int exitCode);

private:
    QString manager;
    QProcess *p;
};

#endif
