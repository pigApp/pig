#ifndef TOPBAR_H
#define TOPBAR_H

#include <QObject>
#include <QtSql>
#include <QString>
#include <QStringList>

#include <QGroupBox>

class TopBar : public QObject
{
    Q_OBJECT

public:
    explicit TopBar(const QString *path = NULL, QObject *parent = 0);
    ~TopBar();

    inline QGroupBox *getGroup() { return group; }

signals:
    void sendData(const QStringList data);

private:
    QSqlDatabase db;
    QStringList data;
    QGroupBox *group;

private slots:
    void query_db(const QString &str = NULL, const bool getData = false);
    void setup_ui();
};

#endif
