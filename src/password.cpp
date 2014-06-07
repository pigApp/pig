#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "password.h"

Password::Password(QObject *parent) :
    QObject(parent)
{
}

Password::~Password()
{
}

bool Password::requirePassword()
{
    #ifdef _WIN32
        QFile file("C:/PIG/.pig/pd.txt");
    #else
        QFile file(QDir::homePath()+"/.pig/.pd");
    #endif
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QString pd = QString(line).toUtf8();
            if(pd != "") {
                file.close(); // TODO: Guardar el valor de pd (hash guardado en el disco) para poder usarlo en rightPassword.
                return true;
                break;
            }
        }
    }
    return false;
}

bool Password::writePassword(QString plain)
{
    const QString hash = hashCalculation(plain);

    #ifdef _WIN32
        QFile file("C:/PIG/.pig/pd.txt");
    #else
        QFile file(QDir::homePath()+"/.pig/.pd");
    #endif
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream write(&file);
        write << hash+"\n";
        file.close();
        return true;
    }
    return false;
}

bool Password::rightPassword(QString plain)
{
    const QString hash = hashCalculation(plain);
    QString diskHash = "202cb962ac59075b964b07152d234b70"; // Usar pd leido en requirePassword().
    if (hash == diskHash)
        return true;
    else
        return false;
}

QString Password::hashCalculation(QString plain)
{
    QByteArray plainB = QString(plain).toUtf8();
    QString hash = QString(QCryptographicHash::hash(plainB,QCryptographicHash::Md5).toHex());
    return hash;
}
