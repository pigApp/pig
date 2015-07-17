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
        index = 0;
        row = 0;
        column = 0;
        offsetData = 0;
        offsetCovers = 0;
        i_covers = 0;
        n_covers = ((*m_data).count() / 15);

        if (ui->btn_cover_vector.size() != 0)
            delete_covers();
    }

    if (n_covers <= 10) {
        i_covers = n_covers;
    } else {
        if ((n_covers - offsetCovers) > 10)
            i_covers = 10;
        else
            i_covers = n_covers - offsetCovers;
    }

    ThreadedSocket *thread[i_covers];

    for(int i = 0; i < i_covers; i++) {  //TODO: AGREGAR '.jpg' A pkg.
        thread[i] = new ThreadedSocket(_PIG_PATH, &(*m_data)[offsetData + 7], &(*m_data)[offsetData + 8], &(*m_data)[offsetData + 1], 0, this);
        connect (thread[i], SIGNAL(sendFile(int, QString)), this, SLOT(add_cover(int, QString)));
        connect (thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
        thread[i]->start();
        offsetData += 15;
    }
    offsetCovers += 10;
}

void View::add_cover(int ID, QString path)
{
    Q_UNUSED(ID);
    int index_cpy = index;

    if (row == 5) {
        row = 0;
        ++column;
    }

    ui->btn_cover_vector.push_back(new QPushButton(this));
    ui->btn_cover_vector.last()->setIconSize(QSize(335, 480));
    ui->btn_cover_vector.last()->setIcon(QIcon(path));
    ui->btn_cover_vector.last()->setFlat(true);
    QObject::connect (ui->btn_cover_vector.last(), &QPushButton::pressed, [=] { show_data(index_cpy); });

    ui->layout_cover->addWidget(ui->btn_cover_vector.last(), column, row);
    ui->layout_cover->update();

    ++index;
    ++row;
}

void View::filter_covers(const QString *filter)
{
    int offsetFilter = 0;

    qDebug() << *filter;

    for (int i = 0; i < ui->btn_cover_vector.size(); i++) {
        if (*filter != (*m_data)[offsetFilter + 3])
            ui->btn_cover_vector.at(i)->setDisabled(true);
        offsetFilter += 15;
    }

    ui->layout_cover->update();
}

void View::delete_covers()
{
    for (int i = 0; i < ui->btn_cover_vector.size(); i++) {
        ui->layout_cover->removeWidget(ui->btn_cover_vector.at(i));
        delete ui->btn_cover_vector.at(i);
    }

    ui->btn_cover_vector.clear();
    ui->layout_cover->update();
}

void View::show_data(int _index)
{
    QObject::connect (ui->btn_hide_data, &QPushButton::clicked, [=] { hide_data(_index); });

    for (int i = 0; i < 5; i++)
        ui->label[i]->setText((*m_data)[((_index * 15) + (i+1))]);

    ui->btn_cover_vector.at(_index)->disconnect();

    ui->layout_data->addWidget(ui->btn_cover_vector.at(_index));
    ui->layout_view->addWidget(ui->group_data);

    ui->group_cover->setDisabled(true);
    ui->group_cover->hide();

    ui->group_data->setEnabled(true);
    ui->group_data->show();

    emit handleTopbar(true);
}

void View::hide_data(int _index)
{
    int r = 0;
    int c = 0;

    if (_index <= 4) {
        r = _index;
        c = 0;
    } else {
        r = (_index - 5); //FIX: HACER BIEN ESTO. getItemPosition
        c = 1;
    }

    ui->layout_data->removeWidget(ui->btn_cover_vector.at(_index));

    ui->layout_cover->addWidget(ui->btn_cover_vector.at(_index), c, r);
    QObject::connect (ui->btn_cover_vector.at(_index), &QPushButton::pressed, [=] { show_data(_index); });

    ui->group_data->setDisabled(true);
    ui->group_data->hide();

    ui->group_cover->setEnabled(true);
    ui->group_cover->show();

    emit handleTopbar(false);
}
