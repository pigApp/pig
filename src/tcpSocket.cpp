#include "tcpSocket.h"

#include <QDataStream>
#include <QTextStream>
#include <QFile>

TcpSocket::TcpSocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    //socket->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(0)); TODO: Setear no keepAlive.
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void TcpSocket::doConnect()
{
    socket->connectToHost(host, 80);
    if(!socket->waitForConnected(10000))
        qDebug() << "Error: " << socket->errorString();
}

void TcpSocket::connected()
{
    QString get = "GET "+url+" HTTP/1.1\r\nHost: "+host+"\r\n\r\n\r\n";
    const QByteArray ba = get.toUtf8();
    const char *request = ba.constData();
    socket->write(request);
}

void TcpSocket::disconnected()
{
    //write();
}

void TcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes;
}

void TcpSocket::readyRead()
{
    while (!socket->atEnd())
        data.append(socket->read(100));
    write();// TODO: Al setear no KeepAlive, write() se llama desde disconnected().
}

void TcpSocket::write()
{    
#ifdef _WIN32
        static QString path = "C:/tmp/pig/";
#else
        static QString path = "/tmp/pig/";
#endif

    if (order == "getUpdateVersion") {
        QString version;
        QString raw(data);
        bool append = false;
        QStringList lines = raw.split("\n");
        foreach(QString line, lines) {
            if (append) {
                //version = line;
                version = "https://dl.shared.com,/g8cj8cnsxk?s=ld,c19e7dbafca6f26c5bafec07907df361,1,/g8cj8cnsxk?s=ld,c19e7dbafca6f26c5bafec07907df361,1,/m9bspu79nd?s=ld,e2462c1f38063a8b14ce102b9a6722e6,1,";
                //break;
                append = false;
            }
            if (line == "\r")
                append = true;
        }
        emit versionReady(version);
    } else if (order == "getPreview") {
        //...
    } else if (order == "getUpdateFiles") {
        QByteArray initData("d8");
        int index = data.indexOf(initData);
        QFile newFile(path+file);
        newFile.open(QIODevice::WriteOnly);
        data.remove(0, index);
        newFile.write(data);
        newFile.close();
        emit fileReady(path, file);
    }
}
