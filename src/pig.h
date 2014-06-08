#ifndef PIG_H
#define PIG_H

#include <QObject>
#include <QApplication>
#include <QVBoxLayout>
#include <QShortcut>
#include <QtSql>

#include "password.h"
#include "update.h"
#include "tcpSocket.h"
#include "torrent.h"
#include "videoplayer.h"

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
    QObject *mRoot;
    Password *mPassword;
    Update* mUpdate;
    TcpSocket mSocket;
    Torrent mTorrent;
    VideoPlayer *mPlayer;

    QSqlDatabase db;
    QShortcut *Esc;
    QShortcut *Quit;

private slots:
    void passwordHandle(QString pass, bool init, bool write);
    void updateHandle();

    void finder();
    void findDb(const QString inputText, QString category, QString pornstar, int offset, bool init);

    void getTorrent(QString host, QString url, QString scenneID);
    void torrentHandle(QString path, QString fileName);
    void openPlayer(QString videoID);
    void closePlayer();

    void errorDb();
    void quit();
};
#endif
