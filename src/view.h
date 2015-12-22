#ifndef VIEW_H
#define VIEW_H

#include "threadedsocket.h"
#include "ui.h"

#include <QWidget>

#include <QTimer>
#include <QShortcut>

namespace Ui {
class View;
}

class View : public QWidget
{
    Q_OBJECT

public:
    explicit View(const QString *PIG_PATH, QPushButton **b_back, QWidget *parent = 0);
    ~View();

signals:
    void sendTopbarState(const bool &setHidden);
    void sendTorrentData(const int &ID, const QStringList **data,
                         const int &sizeData, int scene);

public slots:
    void get_covers(const QStringList *data = NULL, const int &ID = -1);
    void set_filter(const QStringList *filter = NULL);
    void reset_local_covers() {
        onLocalCovers.clear();
        onLocalBackCovers.clear();
    }

private:
    const QString *_PIG_PATH;
    QPushButton **_b_back;

    const QStringList *m_data = NULL;

    bool setLbDownloadHidden;

    Ui::View *ui;

    ThreadedSocket *thread[10] = {};

    QShortcut *sc_back;

    QStringList onLocalCovers;
    QStringList onLocalBackCovers;

    QTimer *t;

    int offsetData, offsetCovers;
    int requiredCovers, requiredRemoteCovers, n_covers;
    int page, n_pages;

    bool hasMoreCovers;

private slots:
    void add_cover(int ID, QString path);
    void delete_covers();
    void init_info(const int &ID, const QString &path);
    void delete_info();
    void pages_handler();
    void set_download_state(bool isCover, bool setHidden);
    bool hasOnLocal(const QString &cover, const QStringList *localList);
};

#endif
