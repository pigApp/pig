#include "authorization.h"

#include <QTextStream>
#include <QCryptographicHash>
#include <QDebug>//

Auth::Auth(const QString *PIG_PATH, bool set_, QWidget *parent) :
    QWidget(parent),
    _set(set_),
    ui(NULL)
{
    file.setFileName(*PIG_PATH+"/.pd");

    this->hide();
}

Auth::~Auth()
{
    if (ui != 0)
        delete ui;
}

void Auth::check()
{
    if (_set) {
        initUi();
    } else {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while (!file.atEnd())
                digest = QString(file.readLine()).toUtf8().simplified();
            file.close();
            initUi();
        } else {
            delete this;
        }
    }
}

void Auth::set(const QString &str)
{
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << calculate(&str).simplified();
        file.close();
        emit setWidget(this);
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
        emit setWidget(this);
    } else {
        qDebug() << "NO-MATCH";
    }
}

const QString Auth::calculate(const QString *plain)
{
    return QString(QCryptographicHash::hash(QString(*plain).toUtf8(),
                                            QCryptographicHash::Md5).toHex());
}

void Auth::initUi()
{
    ui = new Ui::Auth;
    ui->setupUi(this);

    if (!_set) ui->btnReset->hide();

    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        if (_set)
            set((ui->input->selectAll(), ui->input->selectedText()));
        else
            match((ui->input->selectAll(), ui->input->selectedText()));
        ui->input->deselect();
    });
    connect (ui->btnReset, SIGNAL(clicked()), this, SLOT(reset()));

    emit setWidget(this, true);

    this->show();
}
