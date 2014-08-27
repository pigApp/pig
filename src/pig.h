#ifndef PIG_H
#define PIG_H

#include "password.h"
#include "update.h"
#include "torrent.h"//w
#include "videoplayer.h"

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QtSql>
#include <QKeyEvent>
#include <QVBoxLayout>

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent=0);
    ~PIG();

    Q_INVOKABLE void player_handle(const QString absoluteFilePath, bool init, bool sandbox, bool fileReady, bool abort);
    Q_INVOKABLE void quit();

    QWidget *container;
    QVBoxLayout *layout;

public slots:
    void set_root_object(QObject *root);

signals:
    void showUpdateSIGNAL();
    void startSIGNAL();
    void showOutputSIGNAL();
    void listUpdatedSIGNAL();
    void noResultSIGNAL();
    void abortTorrentSIGNAL();
    void checkingFileSIGNAL();
    void fileReadySIGNAL();
    void showErrorDatabaseSIGNAL();

private:
    QObject *mRoot;
    Password *mPassword;
    Update *mUpdate;
    Torrent *mTorrent;//w
    VideoPlayer *mPlayer;

    QSqlDatabase db;

private slots:
    void password_handle(QString pass, bool init, bool write);
    void update_handle();
    void start();
    void find(QString inputText, QString pornstar, QString category, QString quality, QString full, int offset, bool init);
    void torrent_handle(QString magnet, QString str_scene, bool abort);
    void cleanUp();
    void error_database();
};
#endif
