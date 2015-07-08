#ifndef THREADEDSOCKET_H
#define THREADEDSOCKET_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>

class ThreadedSocket : public QThread
{
    Q_OBJECT

public:
    explicit ThreadedSocket(const QString *_PIG_PATH, const QString *host
                            , const QString *url, const QString *pkg = NULL
                            , QObject *parent = 0);
    ~ThreadedSocket();

    void run();

signals:
    void sendData(QString data);
    void sendFile(QString path);
    //void error(QTcpSocket::SocketError socketerror); //TODO: SOCKET ERROR.

private:
    const QString *__PIG_PATH;
    const QString *_host;
    const QString *_url;
    const QString *_pkg;

    QTcpSocket *socket;
    QByteArray data;

private slots:
    void connected();
    void readyRead();
    void processData();
    void disconnected();
};

#endif
