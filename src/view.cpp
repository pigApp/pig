#include "view.h"
#include "threadedsocket.h"

View::View(const QString *PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    ui(new Ui::View)
{
    ui->setupUi(this);
}

View::~View()
{
    delete ui;
}

void View::get(const QStringList *data)
{
    if (data != NULL) {
        m_data = data;
        offset_data = 0;
        offset_movies = 0;
        i_movies = 0;
        n_movies = ((*m_data).count() / 15);
        row = 0;
        column = 0;
    }

    if (n_movies <= 10) {
        i_movies = n_movies;
    } else {
        if ((n_movies - offset_movies) > 10)
            i_movies = 10;
        else
            i_movies = n_movies - offset_movies;
    }

    ThreadedSocket *thread[i_movies];

    for(int i = 0; i < i_movies; i++) {
        thread[i] = new ThreadedSocket(_PIG_PATH, &(*m_data)[offset_data + 7], &(*m_data)[offset_data + 8], &(*m_data)[offset_data + 1], (offset_data), this); //TODO: AGREGAR '.jpg' A pkg.
        connect (thread[i], SIGNAL(sendFile(QString, int)), this, SLOT(add(QString, int)));
        connect (thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
        thread[i]->start();
        offset_data += 15;
    }

    offset_movies += 10;
}

void View::add(QString path, int _offset_data)
{
    if (row == 5) {
        row = 0;
        ++column;
    }

    ui->btn_vector.push_back(new QPushButton(this));
    ui->btn_vector.last()->setIconSize(QSize(335, 480));
    ui->btn_vector.last()->setIcon(QIcon(path));
    ui->btn_vector.last()->setFlat(true);

    QObject::connect (ui->btn_vector.last(), &QPushButton::pressed, [=] { print(_offset_data); });

    ui->layout->addWidget(ui->btn_vector.last(), column, row);
    ui->layout->update();

    ++row;
}

void View::del()
{
    for (int i = 0; i < ui->btn_vector.size(); i++) {
        ui->layout->removeWidget(ui->btn_vector.at(i));
        delete ui->btn_vector.at(i);
    }

    ui->btn_vector.clear();

    ui->layout->update();
}

void View::print(int __offset_data)
{
    int end_data = __offset_data+15;

    while (__offset_data < end_data) {
        qDebug() << (*m_data)[__offset_data];
        ++__offset_data;
    }
}
