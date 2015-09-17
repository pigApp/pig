#ifndef VIEW_H
#define VIEW_H

#include "threadedsocket.h"
#include "ui.h"

#include <QWidget>

#include <QShortcut>

namespace Ui {
class View;
}

class View : public QWidget
{
    Q_OBJECT

public:
    explicit View(const QString *PIG_PATH, QPushButton **b_back, QWidget *parent = 0);
    ~View();

signals:
    void sendTopbarState(const bool &hide);

public slots:
    void get_covers(const QStringList *data = NULL, const int &ID = -1);
    void reset_local_covers() {
        onLocalCovers.clear();
        onLocalBackCovers.clear();
    }
    void set_filter(const QStringList *filter = NULL);

private:
    const QString *_PIG_PATH;
    QPushButton **_b_back;
    const QStringList *m_data = NULL;

    ThreadedSocket *thread[10] = {};

    QStringList onLocalCovers;
    QStringList onLocalBackCovers;

    QShortcut *sc_back;

    Ui::View *ui;

    int offsetData, offsetCovers;
    int requiredCovers, requiredRemoteCovers, n_covers;
    int page, n_pages;

    bool hasMoreCovers;

private slots:
    void add_cover(int ID, QString path);
    void delete_covers();
    void init_info(const int &ID, const QString &path);
    void delete_info();
    void pages_handler();
    bool hasOnLocal(const QString &cover, const QStringList *localList);
};

#endif
