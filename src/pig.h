#ifndef PIG_H
#define PIG_H

#include "topbar.h"
#include "password.h"

#include <QString>
#include <QStringList>

#include <QWidget>
#include <QVBoxLayout>

class PIG : public QWidget
{
    Q_OBJECT

public:
    PIG(QWidget *parent = 0);
    ~PIG();

<<<<<<< HEAD
public slots:
    void set_root_object(QObject *root);
    void password(const bool require, const QString plain
        , const bool check, const bool write);
    void preview(const int id, const QString host, const QString url
        , const QString target
        , const bool ready, const bool success, const bool error, const bool abort);
    void torrent(const QString host, const QString url, const QString target
        , const int scene, const bool abort);
    void find(const QString userInput, const QString category
        , const QString pornstar, const QString quality, const QString full);
    void quit();

signals:
    void sig_show_update();
    void sig_show_news(const QString binaryNews, const QString databaseNews);
    void sig_show_finder();
    void sig_show_db_err();
    void sig_ret_password(const bool require = false, const bool success = false);
    void sig_ret_db(int nMovies, QStringList dataMovies);
    void sig_ret_stream(const int id, const bool ready, const bool success
        , const bool error);

=======
>>>>>>> port
private:
    TopBar *pTopbar;

    QString path;
    QVBoxLayout *layout;

private slots:
<<<<<<< HEAD
    void update();
    void start();
    void cleanup();
    void db_error();
=======
    void authorization(const bool toWrite);
    void showMoviesData(const QStringList &data);
    void setupUi();
>>>>>>> port
};

#endif
