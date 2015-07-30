#include "view.h"
#include "threadedsocket.h"

#include <QDir>

View::View(const QString *PIG_PATH, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    page(1),
    n_pages(0),
    ui(new Ui::View)
{
    ui->setupUi(this);

    QDir dir(*_PIG_PATH+"/tmp/covers");
    onLocalCovers = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    dir.setPath(*_PIG_PATH+"/tmp/covers/back");
    onLocalBackCovers = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    pageHeight = 970; //TODO: PORCENTAJE

    QObject::connect (ui->scrollArea_covers->verticalScrollBar(), &QScrollBar::valueChanged, [&] { pages_handler(); });

    //    QObject::connect (ui->btn_clear, &QPushButton::pressed, [&] {
    //        del_covers();
    //        emit setFilterOnCovers();
    //    });//
}

View::~View()
{
    delete ui;
}

void View::get(const QStringList *data, const int &index)
{
    if (index != -1) {
        int _index = ((index + 1) * 17);
        if (isOnLocal((*m_data)[(_index - 7)], &onLocalBackCovers)) {
            if (ui->group_info != 0) {
                QPixmap pixmap_backCover(*_PIG_PATH+"/tmp/covers/back/"+(*m_data)[(_index - 7)]);
                ui->backCover->setPixmap(pixmap_backCover.scaled(335, 480, Qt::KeepAspectRatio));
            }
        } else {
            ThreadedSocket *thread;
            thread = new ThreadedSocket(_PIG_PATH, &(*m_data)[(_index - 11)],
                                        &(*m_data)[(_index - 8)],
                                        &(*m_data)[(_index - 7)], 0, this);
            QObject::connect (thread, &ThreadedSocket::sendFile, [=] (int ID, QString path) {
                Q_UNUSED(ID);
                if (ui->group_info != 0) {
                    QPixmap pixmap_backCover(path);
                    ui->backCover->setPixmap(pixmap_backCover.scaled(335, 480, Qt::KeepAspectRatio));
                }
                onLocalBackCovers << (*m_data)[(_index - 7)];
            });
            connect (thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            thread->start();
        }
    } else {
        if (data != 0) {
            m_data = data;
            row = 0;
            col = 0;
            offsetData = 0;
            offsetCovers = 0;
            requiredCovers = 0;
            n_covers = ((*m_data).count() / 17);
            filterBase = "";
            filteredCovers.clear();
            hasMoreCovers = true;

            if (ui->btn_vector_covers.size() != 0)
                del_covers();
        }

        if (n_covers <= 10) {
            requiredCovers = n_covers;
        } else {
            if ((n_covers - offsetCovers) > 10)
                requiredCovers = 10;
            else
                requiredCovers = (n_covers - offsetCovers);
        }

        requiredRemoteCovers.clear();

        int _offsetData = offsetData;
        for (int i = 0; i < requiredCovers; i++) {
            if (isOnLocal((*m_data)[(_offsetData + 8)], &onLocalCovers))
                add_cover(-1, *_PIG_PATH+"/tmp/covers/"+(*m_data)[(_offsetData + 8)]);
            else
                requiredRemoteCovers << _offsetData;
            _offsetData += 17;
        }

        if (hasMoreCovers) {
            ThreadedSocket *thread[requiredRemoteCovers.size()];
            for(int i = 0; i < requiredRemoteCovers.size(); i++) {
                thread[i] = new ThreadedSocket(_PIG_PATH, &(*m_data)[(requiredRemoteCovers[i] + 6)],
                                               &(*m_data)[(requiredRemoteCovers[i] + 7)],
                                               &(*m_data)[(requiredRemoteCovers[i] + 8)], 0, this);
                connect (thread[i], SIGNAL(sendFile(int, QString)), this, SLOT(add_cover(int, QString)));
                connect (thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
                thread[i]->start();
                offsetData += 17;
            }
        }
    }
}

void View::add_cover(int ID, QString path)
{
    int index = ui->btn_vector_covers.size();

    if (col == 5) {
        col = 0;
        ++row;
    }

    //TODO: REVISAR EL ORDEN DE LAS TAPAS/CONTRATAPAS/NOMBRES

    ui->btn_vector_covers.push_back(new QPushButton(ui->group_covers));
    ui->btn_vector_covers.last()->setIconSize(QSize(335, 480)); //TODO: PORCENTAJE
    ui->btn_vector_covers.last()->setIcon(QIcon(path));
    ui->btn_vector_covers.last()->setFlat(true);
    QObject::connect (ui->btn_vector_covers.last(), &QPushButton::pressed, [=] { show_info(index, path); });
    //QObject::connect (ui->btn_vector_covers.last(), &QPushButton::pressed, [=] { get(); });

    ui->layout_covers->addWidget(ui->btn_vector_covers.last(), row, col);

    ++col;

    if (ID != -1 && !isOnLocal(path.remove(0, path.lastIndexOf("/")), &onLocalCovers))
        onLocalCovers << (*m_data)[((index+1) * 17) - 9];

    if ((ui->btn_vector_covers.size() - offsetCovers) == requiredCovers) { //FIX: AL BAJAR LA PAGINA NO AGREGA LOS COVERS DESCARGADOS ALA LISTA
        offsetCovers += 10;                                                //FIX: SOLO BAJA EN EL PEDIDO INICIAL
        ++n_pages;
        if ((n_covers - offsetCovers) <= 0)
            hasMoreCovers = false;
    }
}

void View::del_covers()
{
    for (int i = 0; i < ui->btn_vector_covers.size(); i++) {
        ui->layout_covers->removeWidget(ui->btn_vector_covers.at(i));
        delete ui->btn_vector_covers.at(i);
    }

    ui->btn_vector_covers.clear();
    ui->layout_covers->update();
}

void View::show_info(const int &index, const QString &path)
{
    ui->setupInfoUi(index, path, &m_data, this);

    QObject::connect (ui->btn_hideInfo, &QPushButton::pressed, [=] { del_info(); });

    get(NULL, index);

    emit setTopbarState(true);
}

void View::del_info()
{
    delete ui->group_info;
    ui->group_info = NULL;

    ui->scrollArea_covers->setEnabled(true);
    ui->scrollArea_covers->show();

    emit setTopbarState(false);
}

void View::pages_handler()
{
    if ((((ui->scrollArea_covers->height()+10) * page) > ui->group_covers->height()) &&
        (page == n_pages) && hasMoreCovers) {
        ++page;

        get();

        if (requiredRemoteCovers.size() > 4)
            ui->group_covers->setMinimumHeight((ui->group_covers->height() + pageHeight));
        else
            ui->group_covers->setMinimumHeight((ui->group_covers->height() + (pageHeight / 2)));
        
        ui->scrollArea_covers->ensureVisible(0, ui->group_covers->height());
    }
}

bool View::isOnLocal(const QString &cover, const QStringList *localList)
{
    for (int i = 0; i < localList->size(); i++)
        if (cover == (*localList)[i])
            return true;

    return false;
}

void View::set_filter(const QString *filter)
{
    int offsetFilter = 0;
    
    //SET BASE
    if (filterBase == "") {
        if (*filter == "FULL")
            filterBase = "fullMovie";
        else if (*filter == "720p" || *filter == "1080p")
            filterBase = "quality";
        else
            filterBase = "category";
    }

    for (int i = 0; i < ui->btn_vector_covers.size(); i++) {
        if (filterBase == "quality") {
            
            if ((*filter == (*m_data)[(offsetFilter + 3)])) {
                ui->btn_vector_covers.at(i)->setDisabled(true);
            } else {
                filteredCovers.append(i);
                ui->btn_vector_covers.at(i)->setEnabled(true);
            }
        }

        if (filterBase == "fullMovie") {
            if ((*filter == (*m_data)[(offsetFilter + 5)])) {
                ui->btn_vector_covers.at(i)->setDisabled(true);
            } else {
                filteredCovers.append(i);
                ui->btn_vector_covers.at(i)->setEnabled(true);
            }
        }

        if (filterBase == "category") {
            if (*filter != (*m_data)[(offsetFilter + 2)]) {
                ui->btn_vector_covers.at(i)->setDisabled(true);
            } else {
                filteredCovers.append(i);
                ui->btn_vector_covers.at(i)->setEnabled(true);
            }
        }
        offsetFilter += 17;
    }
    qDebug() << filteredCovers;
}









































/*
void View::set_filter(const QString *filter)
{
    int offsetFilter = 0;

    if ((*filter != "ALL") && (*filter != "FULL") && (*filter != "720p") && (*filter != "1080p")) {
       if (filterBase == "category") {
           filterBase = "";
           activeCovers = _activeCovers;
           n_activeCovers = ui->btn_vector_covers.size();
       }
    }


    if (*filter == "ALL") {

        filterBase = "";
        activeCovers = _activeCovers;
        n_activeCovers = ui->btn_vector_covers.size();

        for (int i = 0; i < ui->btn_vector_covers.size(); i++)
            ui->btn_vector_covers.at(i)->setEnabled(true);

    } else {

        for (int i = 0; i < n_activeCovers; i++) {

            if (*filter == "FULL") {

                if ((*filter == (*m_data)[offsetFilter + 6])) {
                    if (filterBase == "") {
                        activeCovers.clear();
                        filterBase = "fullMovie";
                    }
                    if (filterBase == "fullMovie")
                        activeCovers.append(i);

                    ui->btn_vector_covers.at(i)->setEnabled(true);
                } else {
                    ui->btn_vector_covers.at(i)->setDisabled(true);
                }
            } else if (*filter == "720p" || *filter == "1080p") {

                if ((*filter == (*m_data)[offsetFilter + 4])) {
                    if (filterBase == "") {
                        activeCovers.clear();
                        filterBase = "quality";
                    }

                    if (filterBase == "quality")
                        activeCovers.append(i);

                    ui->btn_vector_covers.at(i)->setEnabled(true);
                } else {
                    ui->btn_vector_covers.at(i)->setDisabled(true);
                }
            } else {

                if (*filter == (*m_data)[offsetFilter + 3]) {
                    if (filterBase == "") {
                        activeCovers.clear();
                        filterBase = "category";
                    }
                    if (filterBase == "category")
                        activeCovers.append(i);

                    ui->btn_vector_covers.at(i)->setEnabled(true);
                } else {
                    for (int n = 0; n < activeCovers.size(); n++) {
                        if (n == activeCovers[n])
                            ui->btn_vector_covers.at(n)->setDisabled(true);
                    }
                }

            }

            offsetFilter += 15;
        }
    }

    if (!activeCovers.isEmpty())
        n_activeCovers = activeCovers.size();

    qDebug() << "N_ACTIVES" << n_activeCovers;
    qDebug() << "BASE" << filterBase;
    qDebug() << "";



//    if (*filter == "ALL") {
//        for (int i = 0; i < ui->btn_vector_covers.size(); i++)
//            ui->btn_vector_covers.at(i)->setEnabled(true);
//    } else {
//        for (int i = 0; i < ui->btn_vector_covers.size(); i++) {
//            if (*filter == "FULL") {
//                if ((*filter != (*m_data)[offsetFilter + 6]))
//                    ui->btn_vector_covers.at(i)->setDisabled(true);
//                else
//                    ui->btn_vector_covers.at(i)->setEnabled(true);
//            } else if (*filter == "720p" || *filter == "1080p") {
//                if ((*filter != (*m_data)[offsetFilter + 4]))
//                    ui->btn_vector_covers.at(i)->setDisabled(true);
//                else
//                    ui->btn_vector_covers.at(i)->setEnabled(true);
//            } else {
//                if (*filter != (*m_data)[offsetFilter + 3]) {
//                    ui->btn_vector_covers.at(i)->setDisabled(true);
//                } else {
//                    activeCovers.append(btn_vector_covers.at(i));
//                    ui->btn_vector_covers.at(i)->setEnabled(true);
//                }
//            }
//            offsetFilter += 15;
//        }
//    }

    // TODO: INTERELACIONAR LOS FILTROS

    ui->layout_covers->update();
}
*/
