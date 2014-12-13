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
    void password(const QString arg);

signals:
    void signal_ret_su(int exitCode);

private:
    QString manager;
    QProcess *proc;
};

#endif
