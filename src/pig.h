#ifndef PIG_H
#define PIG_H

#include "topbar.h"
#include "setup.h"
#include "view.h"
#include "ui.h"

#include <QWidget>

#include <QDir>

namespace Ui {
class PIG;
}

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent = 0);
    ~PIG();

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
#ifdef __linux__
    const QString PIG_PATH = QDir::homePath()+"/.pig";
#else
    const QString PIG_PATH = "C:/PIG/.pig";
#endif
    QSqlDatabase db;

    TopBar *topbar;
    Setup *setup;
    View *view;

    bool keep_covers, keep_torrents, keep_movies;
    int torrent_port_1, torrent_port_2;

    Ui::PIG *ui;

private slots:
    void init();
    void init_authorization();
    void init_update();
    void init_topbar();
    void init_viewer(const QStringList *data, const QString *filter);
    void init_setup();
    void error(QString error);
    QHash<QString, QVariant> get_rc();
};

#endif
