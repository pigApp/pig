#ifndef VIEW_H
#define VIEW_H

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
    void setWidget(QWidget *w = NULL, bool add = false);
    void setFilterOnCovers();
    void setTopbarState(const bool &hide);

public slots:
    void get_covers(const QStringList *data = NULL, const int &ID = -1);
    void set_filter(const QStringList *filter = NULL);

private:
    const QString *_PIG_PATH;
    QPushButton **_b_back;
    const QStringList *m_data = NULL;

    QStringList onLocalCovers;
    QStringList onLocalBackCovers;

    QList<int> requiredRemoteCovers;
    QList<int> requiredRemoteCoversID;

    QShortcut *sc_back;

    Ui::View *ui;

    int pageHeight;
    int offsetData, offsetCovers;
    int requiredCovers, n_covers;
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
