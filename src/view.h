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
    void sendWidget(QWidget *w = NULL, bool add = false);
    void handleTopbar(bool hide);

public slots:
    void get(const QStringList *data = NULL);
    void filter_covers(const QString *filter = NULL);//TODO: PUNTERO A FILTER.

private:
    const QString *_PIG_PATH;
    const QStringList *m_data = NULL;

    Ui::View *ui;

    int index;
    int row, column;
    int offsetData, offsetCovers;
    int i_covers, n_covers;

private slots:
    void add_cover(int ID, QString path);
    void delete_covers();
    void show_data(int _index);
    void hide_data(int _index);
};

#endif
