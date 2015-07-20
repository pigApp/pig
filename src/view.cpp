#include "view.h"
#include "threadedsocket.h"

View::View(const QString *PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    ui(new Ui::View)
{
    ui->setupUi(this);

    QObject::connect (ui->btnClear, &QPushButton::pressed, [&] {
        del_covers();
        emit setFilterOnCovers();
    });//
}

View::~View()
{
    delete ui;
}

void View::get(const QStringList *data)
{
    if (data != 0) {
        m_data = data;
        row = 0;
        column = 0;
        offsetData = 0;
        offsetCovers = 0;
        i_covers = 0;
        n_covers = ((*m_data).count() / 15);
        if (ui->btn_covers_vector.size() != 0)
            del_covers();
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
        thread[i] = new ThreadedSocket(_PIG_PATH, &(*m_data)[offsetData + 7],
                                       &(*m_data)[offsetData + 8],
                                       &(*m_data)[offsetData + 1], 0, this);
        connect (thread[i], SIGNAL(sendFile(int, QString)), this, SLOT(add_cover(int, QString)));
        connect (thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
        thread[i]->start();
        offsetData += 15;
    }

    offsetCovers += 10;
}

void View::add_cover(int ID, QString path)
{
    //TODO: ARRAY QUE GUARDE LAS TAPAS YA DESCARGADAS.
    Q_UNUSED(ID);
    int index = ui->btn_covers_vector.size();

    if (row == 5) {
        row = 0;
        ++column;
    }

    ui->btn_covers_vector.push_back(new QPushButton(this));
    ui->btn_covers_vector.last()->setIconSize(QSize(335, 480));
    ui->btn_covers_vector.last()->setIcon(QIcon(path));
    ui->btn_covers_vector.last()->setFlat(true);
    QObject::connect (ui->btn_covers_vector.last(), &QPushButton::pressed, [=] { show_info(index); });

    ui->layoutCovers->addWidget(ui->btn_covers_vector.last(), column, row);
    ui->layoutCovers->update();

    ++row;
}

void View::set_filter(const QString *filter)
{
    int offsetFilter = 0;

    if (*filter == "ALL") {
        for (int i = 0; i < ui->btn_covers_vector.size(); i++)
            ui->btn_covers_vector.at(i)->setEnabled(true);
    } else {
        for (int i = 0; i < ui->btn_covers_vector.size(); i++) {
            if (*filter != (*m_data)[offsetFilter + 3])
                ui->btn_covers_vector.at(i)->setDisabled(true);
            else
                ui->btn_covers_vector.at(i)->setEnabled(true);
            offsetFilter += 15;
        }
    }

    ui->layoutCovers->update();
}

void View::del_covers()
{
    for (int i = 0; i < ui->btn_covers_vector.size(); i++) {
        ui->layoutCovers->removeWidget(ui->btn_covers_vector.at(i));
        delete ui->btn_covers_vector.at(i);
    }

    ui->btn_covers_vector.clear();
    ui->layoutCovers->update();
}

void View::show_info(int index)
{
    ui->setupInfoUi(index, &m_data, this);

    ui->btn_covers_vector.at(index)->disconnect();
    QObject::connect (ui->btnHideInfo, &QPushButton::pressed, [=] { del_info(index); });

    emit setTopbarState(true);
}

void View::del_info(const int &index)
{
    int r = 0;
    int c = 0;

    if (index <= 4) {
        r = index;
        c = 0;
    } else {
        r = (index - 5); //FIX
        c = 1;
    }

    ui->layoutCovers->addWidget(ui->btn_covers_vector.at(index), c, r);
    QObject::connect (ui->btn_covers_vector.at(index), &QPushButton::pressed, [=] { show_info(index); });

    delete ui->groupInfo;

    ui->groupCovers->setEnabled(true);
    ui->groupCovers->show();

    emit setTopbarState(false);
}
