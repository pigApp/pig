#ifndef PIG_H
#define PIG_H

#include <QtSql>
#include <QShortcut>
#include <QVBoxLayout>
#include <QObject>
#include <QApplication>

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

    Q_INVOKABLE void playerHandle(const QString path, const QString file);

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
    Torrent *mTorrent;
    VideoPlayer *mPlayer;

    QSqlDatabase db;
    QShortcut *SpaceBar;
    QShortcut *UpArrow;
    QShortcut *DownArrow;
    QShortcut *Esc;
    QShortcut *Quit;
    int scenne;

private slots:
    void passwordHandle(QString pass, bool init, bool write);
    void updateHandle();

    void finder();
    void findDb(const QString inputText, QString category, QString pornstar, int offset, bool init);

    void getTorrent(QString host, QString url, QString scenneID);
    void torrentHandle(QString path, QString file);

    void closePlayer();
    void errorDb();
    void quit();
};
#endif
