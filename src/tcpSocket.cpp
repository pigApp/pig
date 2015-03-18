#include "tcpSocket.h"

#include <QDataStream>
#include <QDir>
#include <QFile>

TcpSocket::TcpSocket(QTcpSocket *parent) : QTcpSocket(parent)
{
    headerless = false;
    stream = false;
    dumped = false;
    force_abort = false;
    offset = 0;

    connect (this, SIGNAL(connected()), this, SLOT(connected()));
    connect (this, SIGNAL(readyRead()), this, SLOT(ready_to_read()));
    connect (this, SIGNAL(disconnected()), this, SLOT(ret()));
}

TcpSocket::~TcpSocket()
{
    qDebug() << "SOCKET CLOSED";
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
    const QString str = "GET "+urls[offset]
        +" HTTP/1.1\r\nConnection: Close\r\nHost: "+host+"\r\n\r\n\r\n";
    const QByteArray ba = str.toUtf8();
    const char *get = ba.constData();
    writeData(get, ba.size());
}

void TcpSocket::ready_to_read()
{
    timeOut->stop();
    
    while (!atEnd()) 
        data.append(read(100));

    if (!headerless)
        remove_header();

    if (!targets.isEmpty()) {
        file.write(data);
        data.clear();
        if (stream) {
            if (!dumped && (file.size() > 240000)) {
                dumped = true;
                emit sig_ret_stream(id, "", "", "", true, false, false, false);
            } else if (dumped) {
                //emit sig_ret_stream(id, "", "", "", true, file.size(),false, false, false); TODO: Agregar parametro dump a sig_ret_stream.
            }
        }
    }
}

void TcpSocket::remove_header()
{
    int b = 0;
    QByteArray line;
    const QDataStream in(data);
    while (!in.atEnd()) {
        line = in.device()->readLine();
        b += line.size();
        if (line.toHex() == "0d0a") {
            data.remove(0, b);
            headerless = true;
            break;
        }
    }

    if (!targets.isEmpty()) {
#ifdef __linux__ //
    const QString tmp = QDir::homePath()+"/.pig/tmp/";
#else
    const QString tmp = "C:/PIG/.pig/tmp/";
#endif
        file.setFileName(tmp+targets[offset]);
        file.open(QIODevice::WriteOnly);
    }
}

void TcpSocket::ret()
{
    headerless = false;
#ifdef __linux__
    const QString tmp = QDir::homePath()+"/.pig/tmp/";
#else
    const QString tmp = "C:/PIG/.pig/tmp/";
#endif
    
    if (targets.isEmpty()) {
        const QString str(data.constData());
        emit sig_ret_string(&str);
    } else {
        if (!force_abort) {
            file.close();
            files << file.fileName();
            if (offset < (urls.count()-1)) {
                ++offset;
                start();
            } else {
                if (stream)
                    emit sig_ret_stream(id, "", "", "", false, true, false, false);
                else
                    emit sig_ret_files(&tmp, &files);
            }
        }
    }
}

void TcpSocket::socket_error()
{
    timeOut->stop();

    if (stream && !force_abort) {//
        force_abort = true;//
        emit sig_ret_stream(id, "", "", "", false, false, true, false);
    } else {
        emit sig_socket_err();
    }
}





//if (this->error() != 1)
    //socket_error();

/*
    if (targets.isEmpty()) {
        const QString str(data.constData());
        emit sig_ret_string(&str);
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
        //QFile file(tmp+targets);
        //file.open(QIODevice::WriteOnly);
        //file.write(data);
        file.close();
        emit sig_ret_stream(id, "", "", "", true, false);
    }
*/
