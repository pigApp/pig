#ifndef UPDATE_H
#define UPDATE_H

#include "ui.h"

#include <QWidget>

#include <QtSql>

namespace Ui {
class Update;
}

class Update : public QWidget
{
    Q_OBJECT

public:
    explicit Update(const QString* const PIG_PATH, QSqlDatabase *db_, QWidget *parent = 0);
    ~Update();

signals:
    void showWidget(QWidget *w);
    void dbError(QString error);

private:
    const QString* const _PIG_PATH;
    QSqlDatabase *_db;

    QFile file;
    QString origin;
    QString target;
    QString backup;
    QString host;
    QString hostSite;
    QString urlSiteNews;
    QStringList urls, sums, pkgs;

    bool hasNewBin, hasNewDb, hasNewLib;
    int nUnpacked;

    Ui::Update *ui;

    int bin, rel, db, lib;

private slots:
    void get();
    void check(QString data);
    void unpack(int ID, QString path);
    void install();
    void status(const int &exitCode);
    void error(QString error);
    void init_ui();
};

#endif
