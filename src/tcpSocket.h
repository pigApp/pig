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
    QString path;
    QString fileName;

public slots:
    void doConnect();

signals:
    void fileWrited(QString fileName);

private:
    QTcpSocket *socket;
    QByteArray data;

private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void write();
};

#endif
