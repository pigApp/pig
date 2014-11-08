#include "tcpSocket.h"

#include <QDataStream>
#include <QDir>
#include <QFile>

TcpSocket::TcpSocket(QTcpSocket *parent) : QTcpSocket(parent)
{
    connect (this, SIGNAL(connected()), this, SLOT(connected()));
    connect (this, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect (this, SIGNAL(readyRead()), this, SLOT(ready_to_read()));
}

TcpSocket::~TcpSocket()
{
    this->abort();
}

void TcpSocket::start()
{
    abortedPreview = false;

    data.clear();
    this->connectToHost(host, 80);

    timeOut = new QTimer(this);
    timeOut->setSingleShot(true);
    connect (timeOut, SIGNAL(timeout()), this, SLOT(error()));
    timeOut->start(15000);
}

void TcpSocket::connected()
{
    const QString get = "GET "+url+" HTTP/1.1\r\nConnection: Close\r\nHost: "+host+"\r\n\r\n\r\n";
    const QByteArray ba = get.toUtf8();
    const char *request = ba.constData(); 
    this->writeData(request, ba.size());
}

void TcpSocket::disconnected()
{
    write();
}

void TcpSocket::ready_to_read()
{
    timeOut->stop();
    
    while (!this->atEnd())
        data.append(this->read(100));
}

void TcpSocket::write()
{    
#ifdef _WIN32
    const QString path = "C:/PIG/.pig/tmp/";
#else
    const QString path = QDir::homePath()+"/.pig/tmp/";
#endif
    bool endHeader = false;
    QByteArray startPayload;
    const QDataStream in(data);
    while (!in.atEnd()) {
        const QByteArray line = in.device()->readLine();
        if (endHeader) {
            startPayload = line;
            break;
        }
        if (line.toHex() == "0d0a")
            endHeader = true;
    }
    const int header = data.indexOf(startPayload);
    data.remove(0, header);

    QFile target(path+file);
    target.open(QIODevice::WriteOnly);

    if (call == "VERSION") {
        const QByteArray dataFromBase = QByteArray::fromBase64(data);
        const QString version(dataFromBase);
        emit success_version_signal(version);
    } else if (call == "BIN" || call == "DB" || call == "LIB" || call == "NEWS") {
        if (call == "NEWS") {
            const QByteArray dataFromBase = QByteArray::fromBase64(data);
            target.write(dataFromBase);
        } else {
            target.write(data);
        }
        target.close();
        emit success_file_signal(path, file);
    } else if (call == "PREVIEW" && !abortedPreview) {
        target.write(data);
        target.close();
        emit ret_preview_signal("", "", path, file, id, true, false, false);
    }
}

void TcpSocket::abortPreview()
{
    abortedPreview = true;
    timeOut->stop();
    this->abort();
}

void TcpSocket::error()
{
    if (call == "PREVIEW" && !abortedPreview) {
        abortedPreview = true;
        emit ret_preview_signal("", "", "", "", id, false, true, false);
    } else {
        emit fail_socket_signal();
    }
}
