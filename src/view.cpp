#include "view.h"
#include "threadedsocket.h"
#include <QAbstractScrollArea>//


View::View(const QString *PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    ui(new Ui::View)
{
    ui->setupUi(this);

//    QObject::connect (ui->btn_clear, &QPushButton::pressed, [&] {
//        del_covers();
//        emit setFilterOnCovers();
//    });//

    QObject::connect (ui->scrollArea_covers->verticalScrollBar(), &QScrollBar::valueChanged, [&] {
        qDebug() <<  ui->scrollArea_covers->verticalScrollBar()->sliderPosition();
        if (ui->scrollArea_covers->verticalScrollBar()->sliderPosition() > 24 && ui->scrollArea_covers->verticalScrollBar()->sliderPosition() < 26)
            get();
    });
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

        baseFilter = "";
        activeCovers.clear();
        _activeCovers.clear();
        n_activeCovers = 0;
        hasMoreCovers = true;

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

    if (hasMoreCovers) {
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
    }

    offsetCovers += 10;

    if ((n_covers - offsetCovers) <= 0)
        hasMoreCovers = false;
}

void View::add_cover(int ID, QString path)
{
    //TODO: ARRAY QUE GUARDE LAS TAPAS YA DESCARGADAS.
    Q_UNUSED(ID);
    int index = ui->btn_covers_vector.size();

    if (column == 5) {
        column = 0;
        ++row;
    }

    ui->btn_covers_vector.push_back(new QPushButton(ui->group_covers));
    ui->btn_covers_vector.last()->setIconSize(QSize(335, 480)); //TODO: PORCENTAJE
    ui->btn_covers_vector.last()->setIcon(QIcon(path));
    ui->btn_covers_vector.last()->setFlat(true);
    QObject::connect (ui->btn_covers_vector.last(), &QPushButton::pressed, [=] { show_info(index); });
    //QObject::connect (ui->btn_covers_vector.last(), &QPushButton::pressed, [=] { get(); });

    ui->layout_covers->addWidget(ui->btn_covers_vector.last(), row, column);
    ui->layout_covers->update();

    activeCovers.append(ui->btn_covers_vector.size()-1);
    _activeCovers.append(ui->btn_covers_vector.size()-1);
    ++n_activeCovers;

    ++column;
}

void View::set_filter(const QString *filter)
{
    int offsetFilter = 0;

    if ((*filter != "ALL") && (*filter != "FULL") && (*filter != "720p") && (*filter != "1080p")) {
       if (baseFilter == "category") {
           baseFilter = "";
           activeCovers = _activeCovers;
           n_activeCovers = ui->btn_covers_vector.size();
       }
    }


    if (*filter == "ALL") {

        baseFilter = "";
        activeCovers = _activeCovers;
        n_activeCovers = ui->btn_covers_vector.size();

        for (int i = 0; i < ui->btn_covers_vector.size(); i++)
            ui->btn_covers_vector.at(i)->setEnabled(true);

    } else {

        for (int i = 0; i < n_activeCovers; i++) {

            if (*filter == "FULL") {

                if ((*filter == (*m_data)[offsetFilter + 6])) {
                    if (baseFilter == "") {
                        activeCovers.clear();
                        baseFilter = "fullMovie";
                    }
                    if (baseFilter == "fullMovie")
                        activeCovers.append(i);

                    ui->btn_covers_vector.at(i)->setEnabled(true);
                } else {
                    ui->btn_covers_vector.at(i)->setDisabled(true);
                }
            } else if (*filter == "720p" || *filter == "1080p") {

                if ((*filter == (*m_data)[offsetFilter + 4])) {
                    if (baseFilter == "") {
                        activeCovers.clear();
                        baseFilter = "quality";
                    }

                    if (baseFilter == "quality")
                        activeCovers.append(i);

                    ui->btn_covers_vector.at(i)->setEnabled(true);
                } else {
                    ui->btn_covers_vector.at(i)->setDisabled(true);
                }
            } else {

                if (*filter == (*m_data)[offsetFilter + 3]) {
                    if (baseFilter == "") {
                        activeCovers.clear();
                        baseFilter = "category";
                    }
                    if (baseFilter == "category")
                        activeCovers.append(i);

                    ui->btn_covers_vector.at(i)->setEnabled(true);
                } else {
                    for (int n = 0; n < activeCovers.size(); n++) {
                        if (n == activeCovers[n])
                            ui->btn_covers_vector.at(n)->setDisabled(true);
                    }
                }

            }

            offsetFilter += 15;
        }
    }

    if (!activeCovers.isEmpty())
        n_activeCovers = activeCovers.size();

    qDebug() << "N_ACTIVES" << n_activeCovers;
    qDebug() << "BASE" << baseFilter;
    qDebug() << "";



//    if (*filter == "ALL") {
//        for (int i = 0; i < ui->btn_covers_vector.size(); i++)
//            ui->btn_covers_vector.at(i)->setEnabled(true);
//    } else {
//        for (int i = 0; i < ui->btn_covers_vector.size(); i++) {
//            if (*filter == "FULL") {
//                if ((*filter != (*m_data)[offsetFilter + 6]))
//                    ui->btn_covers_vector.at(i)->setDisabled(true);
//                else
//                    ui->btn_covers_vector.at(i)->setEnabled(true);
//            } else if (*filter == "720p" || *filter == "1080p") {
//                if ((*filter != (*m_data)[offsetFilter + 4]))
//                    ui->btn_covers_vector.at(i)->setDisabled(true);
//                else
//                    ui->btn_covers_vector.at(i)->setEnabled(true);
//            } else {
//                if (*filter != (*m_data)[offsetFilter + 3]) {
//                    ui->btn_covers_vector.at(i)->setDisabled(true);
//                } else {
//                    activeCovers.append(btn_covers_vector.at(i));
//                    ui->btn_covers_vector.at(i)->setEnabled(true);
//                }
//            }
//            offsetFilter += 15;
//        }
//    }

    // TODO: INTERELACIONAR LOS FILTROS

    ui->layout_covers->update();
}

void View::del_covers()
{
    for (int i = 0; i < ui->btn_covers_vector.size(); i++) {
        ui->layout_covers->removeWidget(ui->btn_covers_vector.at(i));
        delete ui->btn_covers_vector.at(i);
    }

    ui->btn_covers_vector.clear();
    ui->layout_covers->update();
}

void View::show_info(int index)
{
    ui->setupInfoUi(index, &m_data, this);

    ui->btn_covers_vector.at(index)->disconnect();
    QObject::connect (ui->btn_hideInfo, &QPushButton::pressed, [=] { del_info(index); });

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

    ui->layout_covers->addWidget(ui->btn_covers_vector.at(index), c, r);
    QObject::connect (ui->btn_covers_vector.at(index), &QPushButton::pressed, [=] { show_info(index); });

    delete ui->group_info;

    ui->scrollArea_covers->setEnabled(true);
    ui->scrollArea_covers->show();

    emit setTopbarState(false);
}
