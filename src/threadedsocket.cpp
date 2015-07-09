#include "threadedsocket.h"

#include <QDataStream>

ThreadedSocket::ThreadedSocket(const QString *_PIG_PATH, const QString *host, const QString *url
                               , const QString *pkg, int ID, QObject *parent)
    : QThread(parent)
    , __PIG_PATH(_PIG_PATH)
    , _host(host)
    , _url(url)
    , _pkg(pkg)
    , _ID(ID)
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
    connect (this, SIGNAL(destroyed()), socket, SLOT(deleteLater()), Qt::DirectConnection);

    socket->connectToHost(*_host, 80);

    exec();
}

void ThreadedSocket::connected()
{
    const QByteArray request(QString("GET "+*_url+" HTTP/1.1\r\n"+"Host: "+*_host+"\r\n"
                                     +"Connection: Close\r\n\r\n\r\n").toUtf8());
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

    if (*_pkg == NULL) {
        emit sendData(QString(data.constData()));
    } else {
        QFile file;
        file.setFileName(*__PIG_PATH+"/tmp/"+*_pkg);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        emit sendFile(file.fileName(), _ID);
    }
}

void ThreadedSocket::disconnected()
{
    socket->deleteLater();

    //if (!socket->error)
    processData();

    exit(0);
}
