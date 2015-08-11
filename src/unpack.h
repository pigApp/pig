#ifndef UNPACK_H
#define UNPACK_H

#include <QObject>

class Unpack : public QObject
{
    Q_OBJECT

public:
    explicit Unpack(QObject *parent = 0);
    ~Unpack();

signals:
    void finished(int exitCode);

public slots:
    void unzip(const QString* const PIG_PATH, const QString *path, const QString *sum);
};

#endif
