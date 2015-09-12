#include "view.h"

#include <QDir>
#include <QDebug>//

const int pageHeight = 970;
const int sizeData = 19;

View::View(const QString* const PIG_PATH, QPushButton **b_back, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _b_back(b_back),
    ui(new Ui::View)
{
    ui->setupUi(this);
    
    QObject::connect (ui->sa_covers->verticalScrollBar(), &QScrollBar::valueChanged, [&] { pages_handler(); });

    sc_back = new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(delete_info()));
    sc_back->setEnabled(false);

    QDir target(*_PIG_PATH+"/tmp/covers");
    onLocalCovers = target.entryList(QDir::Files | QDir::NoDotAndDotDot);
    target.setPath(*_PIG_PATH+"/tmp/covers/back");
    onLocalBackCovers = target.entryList(QDir::Files | QDir::NoDotAndDotDot);
}

View::~View()
{
    delete ui;
}

void View::get_covers(const QStringList *data, const int &ID)
{
    if (ID == -1) {

        if (data != 0) {
            m_data = data;
            offsetData = 0;
            offsetCovers = 0;
            requiredCovers = 0;
            n_covers = ((*m_data).count() / sizeData);
            page = 0;
            n_pages = 0;
            hasMoreCovers = true;

            if (ui->v_b_covers.size() != 0)
                delete_covers();
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
        requiredRemoteCoversID.clear();

        for (int i = 0; i < requiredCovers; i++) {
            if (hasOnLocal((*m_data)[(offsetData + 10)], &onLocalCovers)) {
                add_cover(-i, *_PIG_PATH+"/tmp/covers/"+(*m_data)[(offsetData + 10)]); 
            } else {
                requiredRemoteCovers << offsetData;
                requiredRemoteCoversID << i;
            }
            offsetData += sizeData;
        }

        if (hasMoreCovers) {
            for(int i = 0; i < requiredRemoteCovers.size(); i++) {
                thread[i] = new ThreadedSocket(_PIG_PATH, &(*m_data)[(requiredRemoteCovers[i] + 8)],
                                               &(*m_data)[(requiredRemoteCovers[i] + 9)],
                                               &(*m_data)[(requiredRemoteCovers[i] + 10)],
                                               requiredRemoteCoversID[i], this);
                connect (thread[i], SIGNAL(sendFile(int, QString)), this, SLOT(add_cover(int, QString)));
                connect (thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
                QObject::connect (thread[i], &ThreadedSocket::destroyed, [=] { thread[i] = NULL; });

                thread[i]->start();
            }
        }
    } else {
        int _ID = ((ID + 1) * sizeData);

        if (hasOnLocal((*m_data)[(_ID - 7)], &onLocalBackCovers)) {
            if (ui->w_info != 0) {
                QPixmap px_backCover(*_PIG_PATH+"/tmp/covers/back/"+(*m_data)[(_ID - 7)]);
                ui->lb_info_backCover->setPixmap(px_backCover.scaled(335, 480, Qt::KeepAspectRatio));
            }
        } else {
            ThreadedSocket *thread;

            thread = new ThreadedSocket(_PIG_PATH, &(*m_data)[(_ID - 11)],
                                        &(*m_data)[(_ID - 8)],
                                        &(*m_data)[(_ID - 7)], 0, this);
            QObject::connect (thread, &ThreadedSocket::sendFile, [=] (int ID, QString path) {
                Q_UNUSED(ID);
                if (ui->w_info != 0) {
                    QPixmap px_backCover(path);
                    ui->lb_info_backCover->setPixmap(px_backCover.scaled(335, 480, Qt::KeepAspectRatio));
                }
                onLocalBackCovers << (*m_data)[(_ID - 7)];
            });
            connect (thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

            thread->start();
        }
    }
}

void View::add_cover(int ID, QString path)
{
    int _ID = ID;
    int row, col;

    if (_ID >= 0) {
        onLocalCovers << (*m_data)[((((n_pages * 10) + _ID) + 1) * sizeData) - 9];
    } else {
      _ID = -_ID; 
    }

    if (_ID < 5) {
        row = (n_pages * 2);
        col = _ID;
    } else {
        row = ((n_pages * 2) + 1);
        col = (_ID - 5);
    }

    int __ID = ((n_pages * 10) + _ID);
    
    ui->v_b_covers.push_back(new QPushButton(QIcon(path), NULL, ui->w_covers));
    ui->v_b_covers.last()->setIconSize(QSize(335, 480));//TODO: PORCENTAJE
    ui->v_b_covers.last()->setFlat(true);
    
    QObject::connect (ui->v_b_covers.last(), &QPushButton::pressed, [=] { init_info(__ID, path); });

    ui->l_covers->addWidget(ui->v_b_covers.last(), row, col);
    ui->l_covers->update();

    if ((ui->v_b_covers.size() - offsetCovers) == requiredCovers) { //FIX: AL BAJAR LA PAGINA NO AGREGA LOS COVERS DESCARGADOS ALA LISTA
        offsetCovers += 10;                                                   //FIX: SOLO BAJA EN EL PEDIDO INICIAL
        ++n_pages;
        if ((n_covers - offsetCovers) <= 0)
            hasMoreCovers = false;
    }
}

void View::delete_covers()
{
    for (int i = 0; i < 10; i++)
        if (thread[i] != 0)
            thread[i]->exit(0);

    for (int i = 0; i < ui->v_b_covers.size(); i++) {
        ui->l_covers->removeWidget(ui->v_b_covers.at(i));
        delete ui->v_b_covers.at(i);
    }

    ui->v_b_covers.clear();
    ui->l_covers->update();
}

void View::init_info(const int &ID, const QString &path)
{
    ui->setupInfoUi(ID, path, &m_data, sizeData, this);

    get_covers(NULL, ID);

    emit setTopbarState(true);

    connect ((*_b_back), SIGNAL(pressed()), this, SLOT(delete_info()));

    sc_back->setEnabled(true);
}

void View::delete_info()
{
    delete ui->w_info;
    ui->w_info = NULL;

    ui->sa_covers->setEnabled(true);
    ui->sa_covers->show();

    emit setTopbarState(false);

    (*_b_back)->disconnect();
    (*_b_back)->hide();

    sc_back->setEnabled(false);
}

void View::pages_handler()
{
    if ((((ui->sa_covers->height() + 11) * (page + 1)) > ui->w_covers->height()) && //TODO: PORCENTAJE.
        ((page + 1) == n_pages) && hasMoreCovers) {
        ++page;

        get_covers();

        if (requiredRemoteCovers.size() > 4)
            ui->w_covers->setMinimumHeight((ui->w_covers->height() + pageHeight));
        else
            ui->w_covers->setMinimumHeight((ui->w_covers->height() + (pageHeight / 2)));
        
        ui->sa_covers->ensureVisible(0, ui->w_covers->height());
    }
}

bool View::hasOnLocal(const QString &cover, const QStringList *localList)
{
    for (int i = 0; i < localList->size(); i++)
        if (cover == (*localList)[i])
            return true;

    return false;
}

void View::set_filter(const QStringList *filter)
{
    for (int i = 0; i < ui->v_b_covers.size(); i++) {
        if ((*filter)[0] == "CATEGORY") {
            if (!((*m_data)[((i * sizeData) + 6)]).contains((*filter)[1]) &&
                ((*filter)[1] != 0)) {
                ui->v_b_covers.at(i)->setDisabled(true);
            } else {
                ui->v_b_covers.at(i)->setEnabled(true);
            }
        } else if ((*filter)[0] == "PORNSTAR") {
            if (!((*m_data)[((i * sizeData) + 2)]).contains((*filter)[1]) &&
                ((*filter)[1] != 0)) {
                ui->v_b_covers.at(i)->setDisabled(true);
            } else {
                ui->v_b_covers.at(i)->setEnabled(true);
            }
        } else if ((*filter)[0] == "QUALITY") {
            if (!((*filter)[1] == (*m_data)[((i * sizeData) + 3)]) &&
                ((*filter)[1] != 0)) {
                ui->v_b_covers.at(i)->setDisabled(true);
            } else {
                ui->v_b_covers.at(i)->setEnabled(true);
            }
        } else if ((*filter)[0] == "FULLMOVIE") {
            if (((*filter)[1] == (*m_data)[((i * sizeData) + 5)]) &&
                ((*filter)[1] != 0)) {
                ui->v_b_covers.at(i)->setDisabled(true);
            } else {
                ui->v_b_covers.at(i)->setEnabled(true);
            }
        }
    }
}
