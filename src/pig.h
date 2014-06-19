#ifndef PIG_H
#define PIG_H

#include "password.h"
#include "update.h"
#include "tcpSocket.h"
#include "torrent.h"
#include "videoplayer.h"

#include <QObject>
#include <QApplication>
#include <QtSql>
#include <QShortcut>
#include <QMouseEvent>
#include <QVBoxLayout>

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

protected:
    void mouseMoveEvent(QMouseEvent *event);

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

private slots:
    void passwordHandle(QString pass, bool init, bool write);
    void updateHandle();
    void finder();
    void findDb(const QString inputText, QString category, QString pornstar, int offset, bool init);
    void torrentHandle(QString magnetUrl, QString scenneId);
    void closePlayer();
    void errorDb();
    void quit();
};
#endif
