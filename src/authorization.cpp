#include "authorization.h"

#include <QTextStream>
#include <QCryptographicHash>
#include <QTimer>

Authorization::Authorization(const QString* const PIG_PATH, bool set_, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _set(set_),
    hasSet(false),
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

        hasSet = true;
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
            ui->input->setDisabled(true);
            hasSet = true;

            set_icon(false);
        } else {
            ui->input->clear();
            ui->input->setEchoMode(QLineEdit::Normal);
            ui->input->setText("ERROR: CHECK PERMISSIONS ON "+*_PIG_PATH);
            ui->input->setPalette(ui->p_error);
            ui->input->setDisabled(true);

            set_icon(false, true);
        }
    }
}

void Authorization::reset()
{
    if (file.remove()) {
        ui->input->clear();
        ui->input->setEchoMode(QLineEdit::Password);
        ui->input->setEnabled(true);
        hasSet = false;

        set_icon(true);
    } else {
        ui->input->clear();
        ui->input->setEchoMode(QLineEdit::Normal);
        ui->input->setText("ERROR: CHECK PERMISSIONS ON "+*_PIG_PATH);
        ui->input->setPalette(ui->p_error);
        ui->input->setDisabled(true);

        set_icon(true, true);
    }
}

void Authorization::match(const QString &str)
{
    if (calculate(&str) == digest)
        this->deleteLater();
    else
        ui->input->setPalette(ui->p_error);
}

const QString Authorization::calculate(const QString *plain)
{
    return QString(QCryptographicHash::hash(QString(*plain).toUtf8(),
                                            QCryptographicHash::Md5).toHex());
}

void Authorization::set_icon(const bool &hasReset, const bool &hasFailed)
{
    if (hasFailed)
        ui->b_reset->setIcon(QIcon(":/icon-cancel"));
    else
        ui->b_reset->setIcon(QIcon(":/icon-ok"));

    ui->b_reset->setEnabled(true);

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->start(1000);

    if (hasFailed)
        if (hasReset)
            QObject::connect(timer, &QTimer::timeout, [=] { ui->b_reset->setIcon(QIcon(":/icon-reset")); });
        else
            QObject::connect(timer, &QTimer::timeout, [=] {
                ui->b_reset->setIcon(QIcon(":/icon-reset-dark"));
                ui->b_reset->setDisabled(true);
            });
    else
        if (hasReset)
            QObject::connect(timer, &QTimer::timeout, [=] {
                ui->b_reset->setIcon(QIcon(":/icon-reset-dark"));
                ui->b_reset->setDisabled(true);
            });
        else
            QObject::connect(timer, &QTimer::timeout, [=] { ui->b_reset->setIcon(QIcon(":/icon-reset")); });
}

void Authorization::init_ui()
{
    ui = new Ui::Authorization;
    ui->setupUi(_set, hasSet, this);

    QObject::connect (ui->input, &QLineEdit::textChanged, [&] {
        ui->input->setPalette(ui->p);
    });
    QObject::connect (ui->input, &QLineEdit::returnPressed, [&] {
        if (_set)
            set((ui->input->selectAll(), ui->input->selectedText()));
        else
            match((ui->input->selectAll(), ui->input->selectedText()));

        ui->input->deselect();
    });
    connect (ui->b_reset, SIGNAL(clicked()), this, SLOT(reset()));

    if (!_set)
        emit sendWidget(this);
}
