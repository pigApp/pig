#include "viewer.h"
#include "threadedsocket.h"

#include <QPushButton>
#include <QScrollArea>//
#include <QDesktopWidget>//


Viewer::Viewer(const QString *PIG_PATH, QObject *parent)
    : QObject(parent)
    , _PIG_PATH(PIG_PATH)
{
    group = NULL;
}

Viewer::~Viewer()
{

}

void Viewer::get(const QStringList &data)
{
    ThreadedSocket *thread[(data.count()/3)];

    int n = 0;
    for(int i = 0; i < (data.count()/3); i++) {
        thread[i] = new ThreadedSocket(_PIG_PATH, &data[n], &data[n+1], &data[n+2], i, this);
        connect (thread[i], SIGNAL(sendFile(QString, int)), this, SLOT(add(QString, int)));
        connect (thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));
        thread[i]->start();
        n = n + 3;
    }

    if (group == NULL)
        setup_ui();
}

void Viewer::add(QString path, int ID)
{
    Q_UNUSED(ID);

    QPushButton *btn = new QPushButton(group);
    btn->setFlat(true);
    btn->setIcon(QIcon(path));
    btn->setIconSize(QSize(335, 480));

    if (ID < 5)
        layout->addWidget(btn, 0, ID);
    else
        layout->addWidget(btn, 1, ID-5);

    layout->update();
}

void Viewer::setup_ui()
{
    group = new QGroupBox;
    group->setStyleSheet("QGroupBox{ border:0; }");
    group->setFlat(true);

    layout = new QGridLayout(group);

    group->setLayout(layout);

    emit sendGroup(group, true);
}
