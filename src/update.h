#ifndef UPDATE_H
#define UPDATE_H

#include "ui.h"

#include <QWidget>

#include <QtSql>
#include <QLabel>

namespace Ui {
class Update;
}

class Update : public QWidget
{
    Q_OBJECT

public:
    explicit Update(const QString *PIG_PATH, QSqlDatabase *db_, QWidget *parent = 0);
    ~Update();

signals:
    void showWidget(QWidget *w);
    void dbError(QString error);

private:
    const QString *_PIG_PATH;
    QSqlDatabase *_db;

    QFile file;
    QString origin;
    QString target;
    QString backup;
    QString host;
    QString hostSite;
    QString urlSiteNews;
    QStringList urls, sums, pkgs;

    Ui::Update *ui;

    bool hasBin, hasDb, hasLib;
    int bin, rel, db, lib;
    int nUnpacked;

private slots:
    void get();
    void check(QString data);
    void unpack(int ID, QString path);
    void install();
    void status(const int &exitCode);
    void error(const QString &error = NULL);
    void init_ui();
};

#endif
