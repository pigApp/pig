#include "password.h"

#include <QTextStream>

#include <QFont>
#include <QPalette>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>//

Password::Password(QObject *parent, const QString path, const bool toWrite) : QObject(parent)
{
    mToWrite = toWrite;
    file.setFileName(path+".pd");
}

Password::~Password()
{
}

void Password::check()
{
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd())
            digest = QString(file.readLine()).toUtf8();
        file.close();
    }

    if (!digest.isEmpty() || mToWrite)
        setupUi();
    else
        emit finished();
}

bool Password::isMatch(const QString *plain)
{
    if (calculate(&plain) == digest)
        return true;

    return false;
}

bool Password::isWritten(const QString *plain)
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
    return QString(QCryptographicHash::hash(QString(**plain).toUtf8()
           , QCryptographicHash::Md5).toHex());
}

void Password::setupUi()
{
    group = new QGroupBox;
    group->setStyleSheet("QGroupBox{border:0;}");
    group->setFlat(true);

    QFont f(":/font-global");
    f.setPointSize(24); //TODO: CAMBIAR A PORCENTAJE
    f.setCapitalization(QFont::AllUppercase);
    f.setBold(true);

    QBrush b(QColor(0, 0, 0, 255));
    QBrush b1(QColor(255, 255, 255, 255));

    QPalette p;
    p.setBrush(QPalette::Active, QPalette::Button, b);
    p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
    p.setBrush(QPalette::Active, QPalette::Text, b1);
    p.setBrush(QPalette::Active, QPalette::Base, b);
    p.setBrush(QPalette::Active, QPalette::Window, b);
    p.setBrush(QPalette::Active, QPalette::Highlight, b);
    p.setBrush(QPalette::Disabled, QPalette::Button, b);
    p.setBrush(QPalette::Disabled, QPalette::ButtonText, b);
    p.setBrush(QPalette::Disabled, QPalette::Text, b);
    p.setBrush(QPalette::Disabled, QPalette::Base, b);
    p.setBrush(QPalette::Disabled, QPalette::Window, b);
    p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

    QLineEdit *input = new QLineEdit(group);
    input->setFont(f);
    input->setPalette(p);

    QPushButton *reset = new QPushButton("RESET", group);
    reset->setFont(f);
    reset->setPalette(p);
    reset->setFlat(true);

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(input);
    layout->addWidget(reset);

    group->setLayout(layout);

    emit ready();
}
