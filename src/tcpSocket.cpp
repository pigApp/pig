#include <QFile>
#include <QDataStream>

#include "tcpSocket.h"

TcpSocket::TcpSocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void TcpSocket::doConnect()
{
    socket->connectToHost(host, 80);
    if(!socket->waitForConnected(5000))
        qDebug() << "Error: " << socket->errorString();
}

void TcpSocket::connected()
{
    /*
    QString get = "GET "+url+" HTTP/1.1\r\nHost: "+host+"\r\n\r\n"; // TODO: Hacer bien la conversion.
    const char *_get = get.toStdString().c_str();
    socket->write(_get);
    */
    socket->write("GET /torrent/32D5DF418A00AD359F71713484B595A852B719E2.torrent HTTP/1.1\r\nHost: 178.73.198.210\r\n\r\n\r\n");
}

void TcpSocket::disconnected()
{
    write();
}

void TcpSocket::bytesWritten(qint64 bytes)
{

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

    if (order == "getVersion") {
        QString version(data);
        emit versionReady(version);
    } else if (order == "getPreview") {
        //...
    } else if (order == "getTorrent" || order == "getUpdate") {
        QFile file(path+fileName);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        data.remove(0, 330); // TODO: Limpiar el header de otra manera.
        out << (QByteArray) data;
        file.close();
        emit fileReady(path, fileName);
    }
}
