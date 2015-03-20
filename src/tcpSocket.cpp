#include "tcpSocket.h"

#include <QDataStream>
#include <QDir>

TcpSocket::TcpSocket(QTcpSocket *parent) : QTcpSocket(parent)
{
    headerless = false;
    stream = false;
    dumped = false;
    offset = 0;

    setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect (this, SIGNAL(connected()), this, SLOT(connected()));
    connect (this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect (this, SIGNAL(disconnected()), this, SLOT(ret()));
    connect (this, SIGNAL(error(QAbstractSocket::SocketError))
        , this, SLOT(error(QAbstractSocket::SocketError)));
}

TcpSocket::~TcpSocket()
{
    if (file.isOpen())
        file.close();
    abort();
}

void TcpSocket::start()
{
    connectToHost(host, 80);
}

void TcpSocket::connected()
{
    const QString str = "GET "+urls[offset]
        +" HTTP/1.1\r\nConnection: Close\r\nHost: "+host+"\r\n\r\n\r\n";
    const QByteArray ba = str.toUtf8();
    const char *get = ba.constData();
    writeData(get, ba.size());
}

void TcpSocket::readyRead()
{
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
                // Enviar mRoot desde pig.
                // Desconectarlo en el destructor si es distinto a NULL.
                // (*_root)->setProperty("mb_downloaded", mb_downloaded);
            }
        }
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
        file.close();
        files << file.fileName();
        if (offset < (urls.count()-1)) {
            data.clear();
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
#ifdef __linux__
    const QString tmp = QDir::homePath()+"/.pig/tmp/";
#else
    const QString tmp = "C:/PIG/.pig/tmp/";
#endif
        file.setFileName(tmp+targets[offset]);
        file.open(QIODevice::WriteOnly);
    }
}

void TcpSocket::error(QAbstractSocket::SocketError error)
{
    qDebug() << "////" << error;

    if (error != QAbstractSocket::RemoteHostClosedError) { // TODO: Revisar por que da este error al desconectarse.
        if (stream)
            emit sig_ret_stream(id, "", "", "", false, false, true, false);
        else
            emit sig_err();
    }
}
// Tabs hechos.
