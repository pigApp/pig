#include "tcpSocket.h"

#include <QDataStream>
#include <QTextStream>
#include <QFile>

TcpSocket::TcpSocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void TcpSocket::doConnect()
{
    data.clear();

    socket->connectToHost(host, 80);
    if(!socket->waitForConnected(10000))
        qDebug() << "Error: " << socket->errorString();
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

    if (order == "getVersion") {
        bool append = false;
        QString version;
        QString raw(data);
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
    } else if (order == "getFile") {
        //QByteArray initData("d8"); //"\r"
        //int index = data.indexOf(initData);
        QFile target(path+file);
        target.open(QIODevice::WriteOnly);
        //data.remove(0, index);
        target.write(data);
        target.close();
        emit fileReady(path, file);
    }
}
