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

    bool abortedPreview;
    int id;

public slots:
    void start();

signals:
    void signal_ret_str(const QString *const str);
    void signal_ret_files(const QString *const path, const QStringList *const files);
    void signal_ret_preview(const QString, const QString, const QString path, const QString file,
                            const int id, const bool success, const bool abort);
    void signal_fail_socket();

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
    void error();
};

#endif
