#include "password.h"

#include <QTextStream>

#include <QFont>
#include <QPalette>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>//

Password::Password(const QString *path, const bool set, QObject *parent)
    : QObject(parent)
    , _set(set)
{
    group = NULL;

    file.setFileName(*path+".pd");
}

Password::~Password()
{
}

void Password::check()
{
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd())
            digest = QString(file.readLine()).toUtf8().simplified();
        file.close();
    }

    if (!digest.isEmpty() || _set)
        setup_ui();
    else
        emit finished();
}

void Password::set(const QString &str)
{
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << calculate(&str).simplified();
        file.close();
        emit finished();
    } else {
        qDebug() << "NO-SET";
    }
}

void Password::reset()
{
    if (file.remove()) {
        qDebug() << "REMOVED";
    } else {
        qDebug() << "NO-REMOVED";
    }
}

void Password::match(const QString &str)
{
    if (calculate(&str) == digest)
        emit finished();
    else
        qDebug() << "NO-MATCH";
}

const QString Password::calculate(const QString *plain)
{
    return QString(QCryptographicHash::hash(QString(*plain).toUtf8()
                   , QCryptographicHash::Md5).toHex());
}

void Password::setup_ui()
{
    group = new QGroupBox;
    group->setStyleSheet("QGroupBox{border:0;}");
    group->setFlat(true);

    QFont f(":/font-global");
    f.setPointSize(24); //TODO: CAMBIAR A PORCENTAJE
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
    QObject::connect(input, &QLineEdit::returnPressed, [=] {
        if (_set)
            set((input->selectAll(),input->selectedText()));
        else
            match((input->selectAll(),input->selectedText()));
        input->deselect();
    });

    QPushButton *reset = new QPushButton("RESET", group);
    reset->setFont(f);
    reset->setPalette(p);
    reset->setFlat(true);
    if (!_set) reset->hide();
    connect(reset, SIGNAL(clicked()), this, SLOT(reset()));

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(input);
    layout->addWidget(reset);

    group->setLayout(layout);

    emit ready();
}
