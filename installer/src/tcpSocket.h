#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTimer>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit TcpSocket(QTcpSocket *parent = 0);
    ~TcpSocket();

    QString host;
    QString url;
    QString path;

public slots:
    void doConnect();

signals:
    void file_ready(const QString path);
    void error_socket();

private:
    QByteArray data;
    QTimer *timeOut;

private slots:
    void connected();
    void disconnected();
    void ready_to_read();
    void write();
    void error();
};

#endif
