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

    timeOut = new QTimer(this);
    connect(timeOut, SIGNAL(timeout()), this, SLOT(error()));
    timeOut->start(15000);
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
    timeOut->stop();
    
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
        QByteArray dataFromBase = QByteArray::fromBase64(data);
        QString version(dataFromBase);
        emit versionReady(version);
    } else {
        QFile target(path+file);
        target.open(QIODevice::WriteOnly);
        if (file == "news.txt") {
            QByteArray dataFromBase = QByteArray::fromBase64(data);
            target.write(dataFromBase);
        } else {
            target.write(data);
        }
        target.close();
        emit fileReady(path, file);
    }
}

void TcpSocket::error()
{
    timeOut->stop();
    socket->abort();
    emit errorSocket();
}
