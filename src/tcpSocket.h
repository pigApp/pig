#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QObject>

class TcpSocket : public QObject
{
    Q_OBJECT

public:
    explicit TcpSocket(QObject *parent = 0);

    QString host;
    QString url;
    QString file;
    QString order;

public slots:
    void doConnect();

signals:
    void versionReady(QString version);
    void fileReady(QString path, QString file);

private:
    QTcpSocket *socket;
    QByteArray data;

private slots:
    void connected();
    void disconnected();
    void readyRead();
    void write();
};

#endif
