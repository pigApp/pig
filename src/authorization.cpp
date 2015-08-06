#include "authorization.h"

#include <QTextStream>
#include <QCryptographicHash>

Authorization::Authorization(const QString *PIG_PATH, bool set_, QWidget *parent) :
    QWidget(parent),
    _set(set_),
    ui(NULL)
{
    file.setFileName(*PIG_PATH+"/.pd");

    if (_set)
        init_ui();
}

Authorization::~Authorization()
{
    if (ui != 0)
        delete ui;
}

void Authorization::check()
{
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd())
            digest = QString(file.readLine()).toUtf8().simplified();
        file.close();
        init_ui();
    } else {
        delete this;
    }
}

void Authorization::set(const QString &str)
{
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << calculate(&str).simplified();
        file.close();
        ui->input->clear();
        ui->input->setEchoMode(QLineEdit::Normal);
        ui->input->setText("DONE");
        ui->input->setDisabled(true);
    } else {
        qDebug() << "NO-SET";
    }
}

void Authorization::reset()
{
    if (file.remove()) {
        qDebug() << "REMOVED";
    } else {
        qDebug() << "NO-REMOVED";
    }
}

void Authorization::match(const QString &str)
{
    if (calculate(&str) == digest)
        this->deleteLater();
    else
        ui->input->setPalette(ui->palette_error);
}

const QString Authorization::calculate(const QString *plain)
{
    return QString(QCryptographicHash::hash(QString(*plain).toUtf8(),
                                            QCryptographicHash::Md5).toHex());
}

void Authorization::init_ui()
{
    ui = new Ui::Authorization;
    ui->setupUi(_set, this);

    if (_set) ui->button_reset->setHidden(false);

    QObject::connect (ui->input, &QLineEdit::textChanged, [&] {
        ui->input->setPalette(ui->palette);
    });
    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        if (_set)
            set((ui->input->selectAll(), ui->input->selectedText()));
        else
            match((ui->input->selectAll(), ui->input->selectedText()));
        ui->input->deselect();
    });
    connect (ui->button_reset, SIGNAL(clicked()), this, SLOT(reset()));

    if (!_set)
        emit showWidget(this);
}
