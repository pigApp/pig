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
    explicit TopBar(QObject *parent = 0, const QString path = NULL);
    ~TopBar();

    QGroupBox *group; //TODO: LLAMARLO DESDE UNA FUNCION

signals:
    void sendMoviesData(const QStringList data);

private:
    QSqlDatabase db;
    QStringList data;

private slots:
    void request(const QString str);//TODO: USAR UN PUNTERO
    void setupUi();
};

#endif
