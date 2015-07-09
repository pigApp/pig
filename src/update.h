#ifndef UPDATE_H
#define UPDATE_H

#include "su.h"

#include <QObject>
#include <QtSql>
#include <QLabel>
#include <QGroupBox>

class Update : public QObject
{
    Q_OBJECT

public:
    explicit Update(const QString *PIG_PATH, QSqlDatabase *db_, QObject *parent = 0);
    ~Update();

signals:
    void sendGroup(QGroupBox *group = NULL, bool add = false);
    void finished();
    void sig_error();

private:
    const QString *_PIG_PATH;
    QSqlDatabase *_db;

    Su *su;

    QFile file;
    QString origin;
    QString target;
    QString backup;
    QString host;
    QStringList urls, sums, pkgs;
    QLabel *label;
    QGroupBox *group;

    bool hasBin, hasDb, hasLib;
    int bin, rel, db, lib;
    int newBin, newRel, newDb, newLib;
    int nUnpacked;

private slots:
    void get();
    void check(QString data);
    void unpack(QString path, int ID);
    void update();
    void status(int exitCode);
    void error();
    void setup_ui();
};

#endif
