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

    QString host;
    QString url;
    QString file;
    QString call;

    int id;

public slots:
    void start();
    void abortPreview();

signals:
    void success_version_signal(const QString version);
    void success_file_signal(const QString path, const QString file);
    void ret_preview_signal(const QString, const QString, const QString path, const QString file, const int id, const bool success, const bool fail, const bool abort);
    void fail_socket_signal();

private:
    bool abortedPreview;
    QByteArray data;
    QTimer *timeOut;

private slots:
    void connected();
    void disconnected();
    void ready_to_read();
    void write();
    void error();
};

#endif
