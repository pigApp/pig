#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTimer>

#include "tcpSocket.h"

TcpSocket::TcpSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void TcpSocket::doConnect()
{
    qDebug() << "connecting...";

    socket->connectToHost(serverTorrent, 80);

    if(!socket->waitForConnected(5000))
        qDebug() << "Error: " << socket->errorString();

    QTimer::singleShot(5000, this, SLOT(write())); // TODO: Llamar a write() cuando cambia el estado.
}

void TcpSocket::connected()
{
    qDebug() << "connected...";

    /*
    QString get = "GET "+urlTorrent+" HTTP/1.1\r\nHost: "+serverTorrent+"\r\n\r\n"; // TODO: Hacer bien la conversion.
    const char *_get = get.toStdString().c_str();
    socket->write(_get);
    */

    socket->write("GET /torrent/32D5DF418A00AD359F71713484B595A852B719E2.torrent HTTP/1.1\r\nHost: 178.73.198.210\r\n\r\n");
}

void TcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void TcpSocket::bytesWritten(qint64 bytes)
{

}

void TcpSocket::readyRead()
{
    qDebug() << "read...";

    while (!socket->atEnd())
        data.append(socket->read(100)); // TODO: Remover el header.
}

void TcpSocket::write()
{
    qDebug() << "write...";

    QFile file(urlTorrent.replace("/torrent/", "", Qt::CaseSensitive));
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << (QByteArray) data;
    file.close();
}
