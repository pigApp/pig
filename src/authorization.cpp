#include "authorization.h"

#include <QTextStream>
#include <QCryptographicHash>

Authorization::Authorization(const QString* const PIG_PATH, bool set_, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _set(set_),
    setted(false),
    ui(NULL)
{
    file.setFileName(*_PIG_PATH+"/.pd");
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
        setted = true;
        init_ui();
    } else if (_set) {
        init_ui();
    } else {
        delete this;
    }
}

void Authorization::set(const QString &str)
{
    if (!str.isEmpty()) {
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << calculate(&str).simplified();
            file.close();
            ui->input->clear();
            ui->input->setEchoMode(QLineEdit::Normal);
            ui->input->setText("SETTED");
            ui->input->setDisabled(true);
            ui->button_reset->setEnabled(true);
            setted = true;
        } else {
            ui->input->clear();
            ui->input->setEchoMode(QLineEdit::Normal);
            ui->input->setText("ERROR: CHECK PERMISSIONS ON "+*_PIG_PATH);
            ui->input->setDisabled(true);
        }
    }
}

void Authorization::reset()
{
    if (file.remove()) {
        ui->input->clear();
        ui->input->setEchoMode(QLineEdit::Password);
        ui->input->setEnabled(true);
        ui->button_reset->setDisabled(true);
        setted = false;
    } else {
        ui->input->clear();
        ui->input->setEchoMode(QLineEdit::Normal);
        ui->input->setText("ERROR: CHECK PERMISSIONS ON "+*_PIG_PATH);
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
    ui->setupUi(_set, setted, this);

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
