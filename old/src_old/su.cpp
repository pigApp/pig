#include "su.h"

#include <QDir>

Su::Su(QObject *parent) : QObject(parent)
{
    proc = new QProcess(this);
    proc->start("/bin/bash", QStringList() << "-c" << "ls /usr/bin/gksu");
    proc->waitForFinished(500);
    if (proc->exitCode() == 0) {
        manager = "gksu";
    } else {
        proc->start("/bin/bash", QStringList() << "-c" << "ls /usr/bin/kdesu");
        proc->waitForFinished(500);
        if (proc->exitCode() == 0)
            manager = "kdesu";
    }
}

Su::~Su()
{
    delete proc;
}

void Su::update(const QString arg)
{
    if (manager.isEmpty())
        emit sig_ret_su(-1);
    else
        if (manager == "gksu")
            proc->start("/bin/bash", QStringList() << "-c" << manager+" -m 'root password' "+arg);
        else
            proc->start("/bin/bash", QStringList() << "-c" << manager+" -c "+arg);
    connect (proc, SIGNAL(finished(int)), this, SIGNAL(sig_ret_su(int)));
}

void Su::password(const QString)
{
    // TODO: Set permissions to password file.
}
