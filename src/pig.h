#ifndef PIG_H
#define PIG_H

#include "password.h"
#include "update.h"
#include "tcpSocket.h"
#include "torrent.h"
#include "videoplayer.h"

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QtSql>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QVBoxLayout>

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent=0);
    ~PIG();

    Q_INVOKABLE void playerHandle(const QString absoluteFilePath);

    QWidget *container;
    QVBoxLayout *layout;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void setRootObject(QObject *root);

signals:
    void showUpdateSIGNAL();
    void startSIGNAL();
    void showOutputSIGNAL();
    void listUpdatedSIGNAL();
    void noResultSIGNAL();
    void hidePlayerLayerSIGNAL();
    void showErrorDbSIGNAL();

private:
    QObject *mRoot;
    Password *mPassword;
    Update *mUpdate;
    Torrent *mTorrent;
    VideoPlayer *mPlayer;

    QSqlDatabase db;

private slots:
    void passwordHandle(QString pass, bool init, bool write);
    void updateHandle();
    void start();
    void find(const QString inputText, QString category, QString pornstar, int offset, bool init);
    void torrentHandle(QString magnetUrl, QString scenne);
    void closePlayer();
    void errorDb();
    void quit();
};
#endif
