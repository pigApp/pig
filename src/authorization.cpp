#include "authorization.h"

#include <QTextStream>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>//

Auth::Auth(const QString *PIG_PATH, bool set, QObject *parent)
    : QObject(parent)
    , _set(set)
{
    group = NULL;

    file.setFileName(*PIG_PATH+"/.pd");
}

Auth::~Auth()
{
}

void Auth::check()
{
    if (_set) {
        setup_ui();
    } else {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while (!file.atEnd())
                digest = QString(file.readLine()).toUtf8().simplified();
            file.close();
            setup_ui();
        } else {
            emit finished();
        }
    }
}

void Auth::set(const QString &str)
{
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << calculate(&str).simplified();
        file.close();
        emit sendGroup(group);
        emit finished();
    } else {
        qDebug() << "NO-SET";
    }
}

void Auth::reset()
{
    if (file.remove()) {
        qDebug() << "REMOVED";
    } else {
        qDebug() << "NO-REMOVED";
    }
}

void Auth::match(const QString &str)
{
    if (calculate(&str) == digest) {
        emit sendGroup(group);
        emit finished();
    } else {
        qDebug() << "NO-MATCH";
    }
}

const QString Auth::calculate(const QString *plain)
{
    return QString(QCryptographicHash::hash(QString(*plain).toUtf8(), QCryptographicHash::Md5).toHex());
}

void Auth::setup_ui()
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

    QObject::connect (input, &QLineEdit::returnPressed, [=] {
        if (_set)
            set((input->selectAll(),input->selectedText()));
        else
            match((input->selectAll(),input->selectedText()));
        input->deselect();
    });

    QPushButton *btnReset = new QPushButton("RESET", group);
    btnReset->setFont(f);
    btnReset->setPalette(p);
    btnReset->setFlat(true);
    if (!_set) btnReset->hide();

    connect (btnReset, SIGNAL(clicked()), this, SLOT(reset()));

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(input);
    layout->addWidget(btnReset);

    group->setLayout(layout);

    emit sendGroup(group, true);
}
