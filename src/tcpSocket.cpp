#include "tcpSocket.h"

#include <QDataStream>
#include <QDir>
#include <QFile>

TcpSocket::TcpSocket(QTcpSocket *parent) : QTcpSocket(parent)
{
    abortedPreview = false;
    offset = 0;
    connect (this, SIGNAL(connected()), this, SLOT(connected()));
    connect (this, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect (this, SIGNAL(readyRead()), this, SLOT(ready_to_read()));
}

TcpSocket::~TcpSocket()
{
    abort();
}

void TcpSocket::start()
{
    data.clear();

    connectToHost(host, 80);

    timeOut = new QTimer(this);
    timeOut->setSingleShot(true);
    connect (timeOut, SIGNAL(timeout()), this, SLOT(error()));
    timeOut->start(15000);
}

void TcpSocket::connected()
{
    const QString strGet = "GET "+urls[offset]+" HTTP/1.1\r\nConnection: Close\r\nHost: "+host+"\r\n\r\n\r\n";
    const QByteArray baGet = strGet.toUtf8();
    const char *get = baGet.constData();
    writeData(get, baGet.size());
}

void TcpSocket::disconnected()
{
    write();
}

void TcpSocket::ready_to_read()
{
    timeOut->stop();
    
    while (!atEnd())
        data.append(read(100));
}

void TcpSocket::write()
{    
#ifdef __linux__
    const QString path = QDir::homePath()+"/.pig/tmp/";
#else
    const QString path = "C:/PIG/.pig/tmp/";
#endif
    bool header = true;
    QByteArray initPayload;
    const QDataStream in(data);
    while (!in.atEnd()) {
        const QByteArray line = in.device()->readLine();
        if (!header) {
            initPayload = line;
            break;
        }
        if (line.toHex() == "0d0a")
            header = false;
    }
    data.remove(0, data.indexOf(initPayload));

    if (request == "VERSIONS") {
        const QString str(data.constData());
        emit signal_ret_str(&str);
    } else if (request == "UPDATE") {
        QFile file(path+"update_file-"+QString::number(offset)+".zip");
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        files << file.fileName();
        if (offset < (urls.count()-1)) {
            ++offset;
            start();
        } else {
            emit signal_ret_files(&path, &files);
        }
    } else if (request == "PREVIEW" && !abortedPreview) {
        QFile file(path+target);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        emit signal_preview_ret("", "", path, "", id, true, false, false);
    }
}

void TcpSocket::error()
{
    timeOut->stop();

    if (request == "PREVIEW" && !abortedPreview) {
        abortedPreview = true;
        emit signal_preview_ret("", "", "", "", id, false, true, false);
    } else {
        emit signal_fail_socket();
    }
}
