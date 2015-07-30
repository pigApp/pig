#ifndef VIEW_H
#define VIEW_H

#include "ui.h"

#include <QWidget>

namespace Ui {
class View;
}

class View : public QWidget
{
    Q_OBJECT

public:
    explicit View(const QString *PIG_PATH, QWidget *parent = 0);
    ~View();

signals:
    void setWidget(QWidget *w = NULL, bool add = false);
    void setFilterOnCovers();
    void setTopbarState(const bool &hide);

public slots:
    void get(const QStringList *data = NULL, const int &index = -1);
    void set_filter(const QString *filter = NULL);

private:
    const QString *_PIG_PATH;
    const QStringList *m_data = NULL;

    QStringList onLocalCovers;
    QStringList onLocalBackCovers;

    QString filterBase;
    QList<int> requiredRemoteCovers;
    QList<int> filteredCovers;
    
    int page, n_pages, pageHeight;

    Ui::View *ui;

    int row, col;
    int offsetData, offsetCovers;
    int requiredCovers, n_covers;

    bool hasMoreCovers;

private slots:
    void add_cover(int ID, QString path);
    void del_covers();
    void show_info(const int &index, const QString &path);
    void del_info();
    void pages_handler();
    bool isOnLocal(const QString &cover, const QStringList *localList);
};

#endif
