#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QFile>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT

public:                //TODO: RECIBIR PIG_PATH
    explicit TcpSocket(const QString **PIG_PATH, QString *host, QStringList *urls, QStringList *targets
                       , int ID = 0, const bool isStream = false, QTcpSocket *parent = 0);
    ~TcpSocket();

signals:
    void sendStr(const QString *const str);
    void sendFiles(const QStringList *const files);
    //void sig_ret_stream(const int id, const QString host, const QString url , const QString file, const bool ready, const bool success, const bool error, const bool abort);
    void sig_err();

private:
    const QString **__PIG_PATH;
    QString *_host;
    QStringList *_urls;
    QStringList *_targets;
    int _ID;
    bool _isStream;

    QByteArray data;
    QStringList files;
    QFile file;

    int offset;

    bool headerless;
    bool dumped;

private slots:
    void start();
    void connected();
    void readyRead();
    void ret();
    void removeHeader();
    void error(QAbstractSocket::SocketError error);
};

#endif
