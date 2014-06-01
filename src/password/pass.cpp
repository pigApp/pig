#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "pass.h"

Pass::Pass(QObject *parent) :
    QObject(parent)
{
}

Pass::~Pass()
{
}

bool Pass::requirePassCheck()
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
                file.close(); // TODO: Guardar el valor de pd (hash guardado en el disco) para poder usarlo en checkPass.
                return true;
                break;
            }
        }
    }
    return false;
}

bool Pass::writePass(QString plain)
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

bool Pass::checkPass(QString plain)
{
    const QString hash = hashCalculation(plain);
    QString diskHash = "202cb962ac59075b964b07152d234b70"; // Usar pd leido en requirePassCheck().
    if (hash == diskHash)
        return true;
    else
        return false;
}

QString Pass::hashCalculation(QString plain)
{
    QByteArray plainB = QString(plain).toUtf8();
    QString hash = QString(QCryptographicHash::hash(plainB,QCryptographicHash::Md5).toHex());
    return hash;
}
