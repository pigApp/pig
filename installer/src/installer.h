#ifndef INSTALLER_H
#define INSTALLER_H

#include "tcpSocket.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QFont>
#include <QFont>

class Installer : public QWidget
{
    Q_OBJECT

public:
    Installer(QWidget *parent = 0);
    ~Installer();

private:
    TcpSocket *mSocket;

    QVBoxLayout *layout;
    QPushButton *button;
    QFont font;
    QString arch;
    QString path;
    QLabel *status;

private slots:
    void get();
    void check(const QString path);
    void install(const QString path);
    bool moveFiles(bool dir, QString origin, QString target);
    void error();
};

#endif
