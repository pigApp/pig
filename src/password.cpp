#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "password.h"

Password::Password(QObject *parent) : QObject(parent)
{
}

Password::~Password()
{
}

bool Password::require()
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
                file.close(); // TODO: Guardar el valor de pd (hash guardado en el disco) para poder usarlo en right.
                return true;
                break;
            }
        }
    }
    return false;
}

bool Password::write(QString plain)
{
    const QString digest = calculate(plain);

    #ifdef _WIN32
        QFile file("C:/PIG/.pig/pd.txt");
    #else
        QFile file(QDir::homePath()+"/.pig/.pd");
    #endif
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream write(&file);
        write << digest+"\n";
        file.close();
        return true;
    }
    return false;
}

bool Password::right(QString plain)
{
    const QString digest = calculate(plain);
    QString digestLocal = "202cb962ac59075b964b07152d234b70"; // Usar pd leido en requirePassword().
    if (digest == digestLocal)
        return true;
    else
        return false;
}

QString Password::calculate(QString plain)
{
    QByteArray _plain = QString(plain).toUtf8();
    QString digest = QString(QCryptographicHash::hash(_plain,QCryptographicHash::Md5).toHex());
    return digest;
}
