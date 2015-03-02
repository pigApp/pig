#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTimer>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit TcpSocket(QTcpSocket *parent = 0);
    ~TcpSocket();

    QString request;
    QString host;
    QStringList urls;
    QString target;

    bool force_abort;
    int id;

public slots:
    void start();

signals:
    void sig_ret_str(const QString *const str);
    void sig_ret_files(const QString *const tmp, const QStringList *const files);
    void sig_ret_preview(const int id, const QString host, const QString url
        , const QString file , const bool success, const bool abort);
    void sig_socket_err();

private:
    QByteArray data;
    QStringList files;
    QTimer *timeOut;

    int offset;

private slots:
    void connected();
    void disconnected();
    void ready_to_read();
    void write();
    void socket_error();
};

#endif
