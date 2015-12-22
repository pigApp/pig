#include "su.h"

Su::Su(QObject *parent) :
    QObject(parent)
{
    p = new QProcess(this);
    p->start("/bin/bash", QStringList() << "-c" << "ls /usr/bin/gksu");
    p->waitForFinished(500);

    if (p->exitCode() == 0) {
        manager = "gksu";
    } else {
        p->start("/bin/bash", QStringList() << "-c" << "ls /usr/bin/kdesu");
        p->waitForFinished(500);

        if (p->exitCode() == 0)
            manager = "kdesu";
    }
}

Su::~Su()
{
    delete p;
}

void Su::install(const QString arg)
{
    if (manager.isEmpty())
        emit sendExitCode(-1);
    else
        if (manager == "gksu")
            p->start("/bin/bash", QStringList()
                     << "-c" << manager+" -m 'Root password to install PIG update' "+arg);
        else
            p->start("/bin/bash", QStringList()
                     << "-c" << manager+" -c "+arg);

    connect (p, SIGNAL(finished(int)), this, SIGNAL(sendExitCode(int)));
}

void Su::authorization(const QString arg)
{
    Q_UNUSED(arg);
    //TODO: SET PERMISSIONS TO PASSWORD FILE.
}
