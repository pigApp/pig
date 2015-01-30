#ifndef UNZIP_H
#define UNZIP_H

#include <QObject>

class Unzip : public QObject
{
    Q_OBJECT

public:
    explicit Unzip(QObject *parent = 0);
    ~Unzip();

public slots:
    bool unzip(const QString* const *tmp, const QStringList* const *files, const QStringList *sums);
};

#endif
