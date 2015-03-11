#include "tcpSocket.h"

#include <QDataStream>
#include <QDir>
#include <QFile>

TcpSocket::TcpSocket(QTcpSocket *parent) : QTcpSocket(parent)
{
    force_abort = false;
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
    connect (timeOut, SIGNAL(timeout()), this, SLOT(socket_error()));
    timeOut->start(15000);
}

void TcpSocket::connected()
{
    const QString strGet = "GET "+urls[offset]
        +" HTTP/1.1\r\nConnection: Close\r\nHost: "+host+"\r\n\r\n\r\n";
    const QByteArray baGet = strGet.toUtf8();
    const char *get = baGet.constData();
    writeData(get, baGet.size());
}

void TcpSocket::disconnected()
{
    if (this->error() != 1)
        socket_error();
    else
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
    const QString tmp = QDir::homePath()+"/.pig/tmp/";
#else
    const QString tmp = "C:/PIG/.pig/tmp/";
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
        emit sig_ret_str(&str);
    } else if (request == "UPDATE" || request == "TORRENT") {
        QFile file(tmp+"BFF9E2F76C55DA6CFEC07ACD6961BE5607E09B6B_.torrent");//
        //QFile file(tmp+"update_file-"+QString::number(offset)+".zip");
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        files << file.fileName();
        if (offset < (urls.count()-1)) {
            ++offset;
            start();
        } else {
            file.setFileName("BFF9E2F76C55DA6CFEC07ACD6961BE5607E09B6B.torrent");//
            files.clear();//
            files << file.fileName();//
            emit sig_ret_files(&tmp, &files);
        }
    } else if (request == "PREVIEW" && !force_abort) {
        QFile file(tmp+target);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        emit sig_ret_preview(id, "", "", "", true, false);
    }
}

void TcpSocket::socket_error()
{
    timeOut->stop();

    if (request == "PREVIEW" && !force_abort) {
        force_abort = true;
        emit sig_ret_preview(id, "", "", "", false, false);
    } else {
        emit sig_socket_err();
    }
}
