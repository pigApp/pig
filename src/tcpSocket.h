#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTimer>
#include <QFile>//

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
    bool force_abort;
    int id;

public slots:
    void start();

signals:
    void sig_ret_string(const QString *const str);
    void sig_ret_files(const QString *const tmp, const QStringList *const files);
    void sig_ret_stream(const int id, const QString host, const QString url
        , const QString file, const bool ready, const bool success
        , const bool error, const bool abort);
    void sig_socket_err();

private:
    QByteArray data;
    QStringList files;
    QTimer *timeOut;
    QFile file;

    bool headerless;
    bool dumped;
    int offset;

private slots:
    void connected();
    void ready_to_read();
    void remove_header();
    void ret();
    void socket_error();
};

#endif
