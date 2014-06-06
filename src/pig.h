﻿#ifndef PIG_H
#define PIG_H

#include <QObject>
#include <QApplication>
#include <QVBoxLayout>
#include <QShortcut>
#include <QtSql>

#include "password/pass.h"
#include "update/update.h"
#include "socket/tcpSocket.h"
#include "video/videoplayer.h"

class PIG : public QObject
{
    Q_OBJECT

public:
    PIG(QObject *parent = 0);
    ~PIG();

    QWidget *window;
    QWidget *container;
    QVBoxLayout *layout;

public slots:
    void setRootObject(QObject *root);

signals:
    void showUpdateSIGNAL();
    void showFinderSIGNAL();
    void showOutputSIGNAL();
    void listUpdatedSIGNAL();
    void noResultSIGNAL();
    void hidePlayerLayerSIGNAL();
    void showErrorDbMsgSIGNAL();

private:
    QSqlDatabase db;
    QString dbPath;
    QString rootPath;
    QFile *file;
    QShortcut *Esc;
    QShortcut *Quit;
    QStringList _list;

    QObject *mRoot;
    Pass *mPass;
    Update* mUpdate;
    TcpSocket s;
    VideoPlayer *mPlayer;

    short int localDbVersion, localBinVersion, localRelease, row;

private slots:
    void passManager(QString pass, bool init, bool write);
    void update();

    void finder();
    void findDb(const QString inputText, QString category, QString pornstar, int offset, bool init);

    void getTorrent(QString serverTorrent, QString urlTorrent, QString scenneID);

    void openPlayer(QString videoID);
    void closePlayer();

    void errorDb();
    void quit();
};
#endif
