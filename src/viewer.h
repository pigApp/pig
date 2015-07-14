#ifndef VIEWER_H
#define VIEWER_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>

class Viewer : public QObject
{
    Q_OBJECT

public:
    explicit Viewer(const QString *PIG_PATH, QObject *parent = 0);
    ~Viewer();

signals:
    void sendGroup(QGroupBox *group = NULL, bool add = false);

public slots:
    void get(const QStringList &data);

private:
    const QString *_PIG_PATH;

    QGroupBox *group;
    QGridLayout *layout;

private slots:
    void add(QString path, int ID);
    void setup_ui();
};

#endif // VIEWER_H
