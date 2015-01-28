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
    QStringList urls;
    QString target;
    QString request;

    bool abortPreview;
    int id;

public slots:
    void start();

signals:
    void sig_ret_str(const QString *const str);
    void sig_ret_files(const QString *const path, const QStringList *const files);
    void sig_ret_preview(const QString, const QString, const QString path, const QString file,
                         const int id, const bool success, const bool abort);
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
    void err();
};

#endif
