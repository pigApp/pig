#include "view.h"
#include "threadedsocket.h"

#include <QDir>

View::View(const QString* const PIG_PATH, QPushButton **b_back, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _b_back(b_back),
    page(1),
    n_pages(0),
    ui(new Ui::View)
{
    ui->setupUi(this);

    QDir target(*_PIG_PATH+"/tmp/covers");
    onLocalCovers = target.entryList(QDir::Files | QDir::NoDotAndDotDot);
    target.setPath(*_PIG_PATH+"/tmp/covers/back");
    onLocalBackCovers = target.entryList(QDir::Files | QDir::NoDotAndDotDot);

    pageHeight = 970; //TODO: PORCENTAJE

    QObject::connect (ui->sa_covers->verticalScrollBar(), &QScrollBar::valueChanged, [&] { pages_handler(); });

    //    QObject::connect (ui->b_clear, &QPushButton::pressed, [&] {
    //        del_covers();
    //        emit setFilterOnCovers();
    //    });//
}

View::~View()
{
    delete ui;
}

void View::get_covers(const QStringList *data, const int &index)
{
    if (index != -1) {
        int _index = ((index + 1) * 17);
        if (isOnLocal((*m_data)[(_index - 7)], &onLocalBackCovers)) {
            if (ui->g_info != 0) {
                QPixmap px_backCover(*_PIG_PATH+"/tmp/covers/back/"+(*m_data)[(_index - 7)]);
                ui->backCover->setPixmap(px_backCover.scaled(335, 480, Qt::KeepAspectRatio));
            }
        } else {
            ThreadedSocket *thread;
            thread = new ThreadedSocket(_PIG_PATH, &(*m_data)[(_index - 11)],
                                        &(*m_data)[(_index - 8)],
                                        &(*m_data)[(_index - 7)], 0, this);
            QObject::connect (thread, &ThreadedSocket::sendFile, [=] (int ID, QString path) {
                Q_UNUSED(ID);
                if (ui->g_info != 0) {
                    QPixmap px_backCover(path);
                    ui->backCover->setPixmap(px_backCover.scaled(335, 480, Qt::KeepAspectRatio));
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

            if (ui->b_vector_covers.size() != 0)
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
    int index = ui->b_vector_covers.size();

    if (col == 5) {
        col = 0;
        ++row;
    }

    //TODO: REVISAR EL ORDEN DE LAS TAPAS/CONTRATAPAS/NOMBRES

    ui->b_vector_covers.push_back(new QPushButton(QIcon(path), NULL, ui->g_covers));
    ui->b_vector_covers.last()->setIconSize(QSize(335, 480)); //TODO: PORCENTAJE
    ui->b_vector_covers.last()->setFlat(true);
    QObject::connect (ui->b_vector_covers.last(), &QPushButton::pressed, [=] { show_info(index, path); });
    //QObject::connect (ui->b_vector_covers.last(), &QPushButton::pressed, [=] { get_covers(); });

    ui->l_covers->addWidget(ui->b_vector_covers.last(), row, col);

    ++col;

    if (ID != -1 && !isOnLocal(path.remove(0, path.lastIndexOf("/")), &onLocalCovers))
        onLocalCovers << (*m_data)[((index+1) * 17) - 9];

    if ((ui->b_vector_covers.size() - offsetCovers) == requiredCovers) { //FIX: AL BAJAR LA PAGINA NO AGREGA LOS COVERS DESCARGADOS ALA LISTA
        offsetCovers += 10;                                                   //FIX: SOLO BAJA EN EL PEDIDO INICIAL
        ++n_pages;
        if ((n_covers - offsetCovers) <= 0)
            hasMoreCovers = false;
    }
}

void View::del_covers()
{
    for (int i = 0; i < ui->b_vector_covers.size(); i++) {
        ui->l_covers->removeWidget(ui->b_vector_covers.at(i));
        delete ui->b_vector_covers.at(i);
    }

    ui->b_vector_covers.clear();
    ui->l_covers->update();
}

void View::show_info(const int &index, const QString &path)
{
    ui->setupInfoUi(index, path, &m_data, this);

    get_covers(NULL, index);

    emit setTopbarState(true);

    connect ((*_b_back), SIGNAL(pressed()), this, SLOT(del_info()));
    (*_b_back)->show();
}

void View::del_info()
{
    delete ui->g_info;
    ui->g_info = NULL;

    ui->sa_covers->setEnabled(true);
    ui->sa_covers->show();

    emit setTopbarState(false);

    (*_b_back)->disconnect();
    (*_b_back)->hide();
}

void View::pages_handler()
{
    if ((((ui->sa_covers->height()+10) * page) > ui->g_covers->height()) &&
        (page == n_pages) && hasMoreCovers) {
        ++page;

        get_covers();

        if (requiredRemoteCovers.size() > 4)
            ui->g_covers->setMinimumHeight((ui->g_covers->height() + pageHeight));
        else
            ui->g_covers->setMinimumHeight((ui->g_covers->height() + (pageHeight / 2)));
        
        ui->sa_covers->ensureVisible(0, ui->g_covers->height());
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

    for (int i = 0; i < ui->b_vector_covers.size(); i++) {
        if (filterBase == "quality") {
            
            if ((*filter == (*m_data)[(offsetFilter + 3)])) {
                ui->b_vector_covers.at(i)->setDisabled(true);
            } else {
                filteredCovers.append(i);
                ui->b_vector_covers.at(i)->setEnabled(true);
            }
        }

        if (filterBase == "fullMovie") {
            if ((*filter == (*m_data)[(offsetFilter + 5)])) {
                ui->b_vector_covers.at(i)->setDisabled(true);
            } else {
                filteredCovers.append(i);
                ui->b_vector_covers.at(i)->setEnabled(true);
            }
        }

        if (filterBase == "category") {
            if (*filter == "ALL") {
                filteredCovers.append(i);
                ui->b_vector_covers.at(i)->setEnabled(true);
            } else if (*filter != (*m_data)[(offsetFilter + 2)]) {
                ui->b_vector_covers.at(i)->setDisabled(true);
            } else {
                filteredCovers.append(i);
                ui->b_vector_covers.at(i)->setEnabled(true);
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
           n_activeCovers = ui->b_vector_covers.size();
       }
    }


    if (*filter == "ALL") {

        filterBase = "";
        activeCovers = _activeCovers;
        n_activeCovers = ui->b_vector_covers.size();

        for (int i = 0; i < ui->b_vector_covers.size(); i++)
            ui->b_vector_covers.at(i)->setEnabled(true);

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

                    ui->b_vector_covers.at(i)->setEnabled(true);
                } else {
                    ui->b_vector_covers.at(i)->setDisabled(true);
                }
            } else if (*filter == "720p" || *filter == "1080p") {

                if ((*filter == (*m_data)[offsetFilter + 4])) {
                    if (filterBase == "") {
                        activeCovers.clear();
                        filterBase = "quality";
                    }

                    if (filterBase == "quality")
                        activeCovers.append(i);

                    ui->b_vector_covers.at(i)->setEnabled(true);
                } else {
                    ui->b_vector_covers.at(i)->setDisabled(true);
                }
            } else {

                if (*filter == (*m_data)[offsetFilter + 3]) {
                    if (filterBase == "") {
                        activeCovers.clear();
                        filterBase = "category";
                    }
                    if (filterBase == "category")
                        activeCovers.append(i);

                    ui->b_vector_covers.at(i)->setEnabled(true);
                } else {
                    for (int n = 0; n < activeCovers.size(); n++) {
                        if (n == activeCovers[n])
                            ui->b_vector_covers.at(n)->setDisabled(true);
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
//        for (int i = 0; i < ui->b_vector_covers.size(); i++)
//            ui->b_vector_covers.at(i)->setEnabled(true);
//    } else {
//        for (int i = 0; i < ui->b_vector_covers.size(); i++) {
//            if (*filter == "FULL") {
//                if ((*filter != (*m_data)[offsetFilter + 6]))
//                    ui->b_vector_covers.at(i)->setDisabled(true);
//                else
//                    ui->b_vector_covers.at(i)->setEnabled(true);
//            } else if (*filter == "720p" || *filter == "1080p") {
//                if ((*filter != (*m_data)[offsetFilter + 4]))
//                    ui->b_vector_covers.at(i)->setDisabled(true);
//                else
//                    ui->b_vector_covers.at(i)->setEnabled(true);
//            } else {
//                if (*filter != (*m_data)[offsetFilter + 3]) {
//                    ui->b_vector_covers.at(i)->setDisabled(true);
//                } else {
//                    activeCovers.append(b_vector_covers.at(i));
//                    ui->b_vector_covers.at(i)->setEnabled(true);
//                }
//            }
//            offsetFilter += 15;
//        }
//    }

    // TODO: INTERELACIONAR LOS FILTROS

    ui->l_covers->update();
}
*/
