#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QFile>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit TcpSocket(QTcpSocket *parent = 0);
    ~TcpSocket();

    QString host;
    QStringList urls;
    QStringList targets;

    bool stream;
    int id;

public slots:
    void start();

signals:
    void sig_ret_string(const QString *const str);
    void sig_ret_files(const QString *const tmp, const QStringList *const files);
    void sig_ret_stream(const int id, const QString host, const QString url
        , const QString file, const bool ready, const bool success
        , const bool error, const bool abort);
    void sig_err();

private:
    QByteArray data;
    QFile file;
    QStringList files;

    bool headerless;
    bool dumped;
    int offset;

private slots:
    void connected();
    void readyRead();
    void ret();
    void remove_header();
    void error(QAbstractSocket::SocketError error);
};

#endif
