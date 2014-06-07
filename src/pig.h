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
    QSqlDatabase db;
    QString dbPath;
    QString tmpPath;
    QFile *file;
    QShortcut *Esc;
    QShortcut *Quit;
    QStringList _list;

    QObject *mRoot;
    Password *mPass;
    Update* mUpdate;
    TcpSocket mSocket;
    Torrent mTorrent;
    VideoPlayer *mPlayer;

    short int localDbVersion, localBinVersion, localRelease, row;

private slots:
    void passwordManager(QString pass, bool init, bool write);
    void update();

    void finder();
    void findDb(const QString inputText, QString category, QString pornstar, int offset, bool init);

    void getTorrent(QString serverTorrent, QString urlTorrent, QString scenneID);
    void torrentManager(QString file);
    void openPlayer(QString videoID);
    void closePlayer();

    void errorDb();
    void quit();
};
#endif
