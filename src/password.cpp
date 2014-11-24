#include "password.h"

#include <QTextStream>
#include <QDir>

Password::Password(QObject *parent) : QObject(parent)
{
#ifdef __linux__
    const QString target = QDir::homePath()+"/.pig/.pd";
#else
    const QString target = "C:/PIG/.pig/.pd";
#endif
    file.setFileName(target);
}

Password::~Password()
{
}

bool Password::check(const QString *plain)
{
    QString digest;
    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        while (!file.atEnd()) {
            digest = QString(file.readLine()).toUtf8();
        }
    }
    file.close();

    if (calculate(&plain) == digest)
        return true;
    return false;
}

bool Password::write(const QString *plain)
{
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream write(&file);
        write << calculate(&plain);
        file.close();
        return true;
    }
    return false;
}

const QString Password::calculate(const QString **plain)
{
    return QString(QCryptographicHash::hash(QString(**plain).toUtf8(),QCryptographicHash::Md5).toHex());
}
