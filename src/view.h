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
    void get(const QStringList *data = NULL);
    void set_filter(const QString *filter = NULL);

private:
    const QString *_PIG_PATH;
    const QStringList *m_data = NULL;

    Ui::View *ui;

    int row, column;
    int offsetData, offsetCovers;
    int i_covers, n_covers;

private slots:
    void add_cover(int ID, QString path);
    void del_covers();
    void show_info(int index);
    void del_info(const int &index);
};

#endif
