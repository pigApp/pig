#ifndef PIG_H
#define PIG_H

#include "topbar.h"
#include "setup.h"
#include "view.h"
#include "player.h"
#include "torrent.h"
#include "error.h"
#include "ui.h"

#include <QWidget>

#include <QDir>
#include <QShortcut>

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
    TopBar *topbar;
    Setup *setup;
    View *view;
    Player *player;
    Torrent *torrent;
    Error *error;

    bool keep_covers, keep_torrents, keep_movies;
    int torrent_port_1, torrent_port_2;

    Ui::PIG *ui;

    QShortcut *sc_backSetup;
    QShortcut *sc_backMovie;
    QShortcut *sc_quit;

    QSqlDatabase db;

#ifdef __linux__
    const QString PIG_PATH = QDir::homePath()+"/.pig";
#else
    const QString PIG_PATH = "C:/PIG/.pig";
#endif

private slots:
    bool init();
    void init_authorization();
    void init_update();
    void init_topbar();
    void init_view(const QStringList *data, const QStringList *filter);
    void init_movie(const int &ID = 0, const QStringList **data = NULL,
                    const int &sizeData = 0, int scene = 0);
    void init_setup();
    void init_error(QString errorMsg);
    QHash<QString, QVariant> get_rc();
};

#endif
