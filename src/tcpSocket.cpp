#include "tcpSocket.h"

#include <QDataStream>
#include <QFile>

TcpSocket::TcpSocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void TcpSocket::doConnect()
{
    data.clear();
    socket->connectToHost(host, 80);
}

void TcpSocket::connected()
{
    QString get = "GET "+url+" HTTP/1.1\r\nConnection: Close\r\nHost: "+host+"\r\n\r\n\r\n";
    const QByteArray ba = get.toUtf8();
    const char *request = ba.constData();
    socket->write(request);
}

void TcpSocket::disconnected()
{
     write();
}

void TcpSocket::readyRead()
{
    while (!socket->atEnd())
        data.append(socket->read(100));
}

void TcpSocket::write()
{    
#ifdef _WIN32
        static QString path = "C:/tmp/pig/";
#else
        static QString path = "/tmp/pig/";
#endif
    bool endHeader = false;
    QByteArray startPayload;
    QDataStream in(data);
    while (!in.atEnd()) {
        QByteArray line = in.device()->readLine();
        if (endHeader) {
            startPayload = line;
            break;
        }
        if (line.toHex() == "0d0a")
            endHeader = true;
    }
    int header = data.indexOf(startPayload);
    data.remove(0, header);

    if (order == "getVersion") {
        QString version(data);
        emit versionReady(version);
    } else {
        QFile target(path+file);
        target.open(QIODevice::WriteOnly);
        target.write(data);
        target.close();
        emit fileReady(path, file);
    }
}
