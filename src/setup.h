#ifndef SETUP_H
#define SETUP_H

#include "ui.h"

#include <QWidget>

#include <QtSql>

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT

public:
    explicit Setup(const QString* const PIG_PATH, bool *keep_covers,
                   bool *keep_torrents, bool *keep_movies, int *torrent_port_1,
                   int *torrent_port_2, QSqlDatabase *db, QWidget *parent = 0);
    ~Setup();

signals:

private:
    const QString* const _PIG_PATH;
    QSqlDatabase *_db;

    Ui::Setup *ui;

private slots:
    void set_rc(const QString &option, const QVariant &value);
    void clean_folder(const QString &folder);
};

#endif
