#ifndef TOPBAR_H
#define TOPBAR_H

#include <QObject>
#include <QtSql>
#include <QGroupBox>

class TopBar : public QObject
{
    Q_OBJECT

public:
    explicit TopBar(const QString *PIG_PATH = NULL, QObject *parent = 0);
    ~TopBar();

    inline QGroupBox *getGroup() { return group; }

signals:
    void sendData(const QStringList data);
    void sendGroup(QGroupBox *group = NULL, const bool add = false);

private:
    QSqlDatabase db;
    QGroupBox *group;

private slots:
    QGroupBox *filterGroup(const QString &filter, const QStringList &filterData);
    void query(const QString &str = NULL, const bool getData = false, const bool getFilter = false);
    void setup_ui();
};

#endif
