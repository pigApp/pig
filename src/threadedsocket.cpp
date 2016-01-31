#include "threadedsocket.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>//

ThreadedSocket::ThreadedSocket(const QString* const PIG_PATH, const QString *host,
                               const QString *url, const QString *pkg, int ID, QObject *parent) :
    QThread(parent),
    _PIG_PATH(PIG_PATH),
    _host(host),
    _url(url),
    _pkg(pkg),
    _ID(ID)
{
}

ThreadedSocket::~ThreadedSocket()
{
}

void ThreadedSocket::run()
{
    socket = new QTcpSocket();
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    connect (socket, SIGNAL(connected()), this, SLOT(connected()), Qt::DirectConnection);
    connect (socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect (socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
    connect (socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
             SLOT(error(QAbstractSocket::SocketError)), Qt::DirectConnection);

    connect (this, SIGNAL(destroyed()), socket, SLOT(deleteLater()), Qt::DirectConnection);

    socket->connectToHost(*_host, 80);

    exec();
}

void ThreadedSocket::connected()
{
    const QByteArray request(QString("GET "+*_url+" HTTP/1.1\r\n"+"Host: "+*_host+"\r\n"+
                                     "Connection: Close\r\n\r\n\r\n").toUtf8());
    socket->write(request);
}

void ThreadedSocket::readyRead()
{
    data.append(socket->readAll());
}

void ThreadedSocket::processData()
{
    int bytes = 0;
    QByteArray line;
    const QDataStream in(data);

    while (!in.atEnd()) {
        line = in.device()->readLine();
        bytes += line.size();

        if (line.toHex() == "0d0a") {
            data.remove(0, bytes);
            break;
        }
    }

    if (*_pkg == 0) {
        emit sendData(QString(data.constData()));
    } else {
        QFile file;
        if (_pkg->endsWith(".zip"))
            file.setFileName(*_PIG_PATH+"/tmp/update/"+*_pkg);
        else if (_pkg->endsWith("_front.jpg"))
            file.setFileName(*_PIG_PATH+"/tmp/covers/"+*_pkg);
        else if (_pkg->endsWith("_back.jpg"))
            file.setFileName(*_PIG_PATH+"/tmp/covers/back/"+*_pkg);
        else if (_pkg->endsWith(".torrent"))
            file.setFileName(*_PIG_PATH+"/tmp/torrents/"+*_pkg);
        else
            file.setFileName(*_PIG_PATH+"/tmp/"+*_pkg);

        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();

        emit sendFile(_ID, file.fileName());
    }
}

void ThreadedSocket::disconnected()
{
    socket->deleteLater();
    processData();
    exit(0);
}

void ThreadedSocket::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;

    if ((socketError != 1) && (socketError != -1)) {
        emit sendError("NETWORK ERROR");
        socket->deleteLater();
        exit(0);
    }
}
