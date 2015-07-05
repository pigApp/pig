#include "tcpsocket.h"

#include <QDataStream>
#include <QDir>

#include <QDebug>//

TcpSocket::TcpSocket(const QString **PIG_PATH, QString *host, QStringList *urls
                     , QStringList *targets, int ID, const bool isStream, QTcpSocket *parent)
    : QTcpSocket(parent)
    ,__PIG_PATH(PIG_PATH)
    ,_host(host)
    ,_urls(urls)
    ,_targets(targets)
    ,_ID(ID)
    ,_isStream(isStream)
{
    offset = 0;
    headerless = false;
    dumped = false;

    setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect (this, SIGNAL(connected()), this, SLOT(connected()));
    connect (this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect (this, SIGNAL(disconnected()), this, SLOT(ret()));
    connect (this, SIGNAL(error(QAbstractSocket::SocketError)) , this, SLOT(error(QAbstractSocket::SocketError)));

    start();
}

TcpSocket::~TcpSocket()
{
    qDebug() << "DELETE SOCKET";
    if (file.isOpen())
        file.close();
}

void TcpSocket::start()
{
    connectToHost(*_host, 80);
}

void TcpSocket::connected()
{
    const QString str = "GET "+(*_urls)[offset]+" HTTP/1.1\r\n"+
                        "Host: "+*_host+"\r\n"+
                        "Connection: Close\r\n\r\n\r\n"; //TODO: PEDIR QUE NO ENVIE EL HEADER. Accept: ''.
    const QByteArray ba = str.toUtf8();
    const char *get = ba.constData();

    writeData(get, ba.size());
}

void TcpSocket::readyRead()
{
    while (!atEnd()) 
        data.append(read(100));

    if (!headerless)
        removeHeader();

    if (!(*_targets).isEmpty()) {
        file.write(data);
        data.clear();
        if (_isStream && !dumped && (file.size() > 240000)) {
            dumped = true;
            //emit sig_ret_stream(id, "", "", "", true, false, false, false);
        }
    }
}

void TcpSocket::ret()
{
    headerless = false;

    if ((*_targets).isEmpty()) {
        const QString str(data.constData());
        emit sendStr(&str);
        deleteLater();// TODO: ELIMINAR LA CLASE UNA VEZ USADA.
    } else {
        file.close();
        files << file.fileName();
        if (offset < (((*_urls).count())-1)) {
            data.clear();
            ++offset;
            start();
        } else {
            if (_isStream)
                //emit sig_ret_stream(id, "", "", "", false, true, false, false);
                qDebug() << "IS STREAM";
            else
                emit sendFiles(&files);
        }
    }
}

void TcpSocket::removeHeader()
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

    if (!(*_targets).isEmpty()) {
        file.setFileName(**__PIG_PATH+"/tmp/"+(*_targets)[offset]);
        file.open(QIODevice::WriteOnly);
    }
}

void TcpSocket::error(QAbstractSocket::SocketError error)
{
    qDebug() << "////" << error;

    if (error != QAbstractSocket::RemoteHostClosedError) { // TODO: Revisar por que da este error al desconectarse.
        if (_isStream)
            //emit sig_ret_stream(id, "", "", "", false, false, true, false);
            qDebug() << "IS STREAM";
        else
            emit sig_err();
    }
}
// Tabs hechos.
