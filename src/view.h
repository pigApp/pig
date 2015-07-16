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

public slots:
    void get(const QStringList *data = NULL);

private:
    const QString *_PIG_PATH;
    const QStringList *m_data = NULL;

    Ui::View *ui;

    int row, column;
    int i_movies, n_movies;
    int offset_data, offset_movies;

private slots:
    void add(QString path, int _offset_data);
    void del();
    void print(int __offset_data);
};

#endif
