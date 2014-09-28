#ifndef INSTALLER_H
#define INSTALLER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QFont>

class Installer : public QWidget
{
    Q_OBJECT

public:
    Installer(QWidget *parent = 0);
    ~Installer();

private:
    QVBoxLayout *layout;
    QPushButton *button;
    QFont font;
    QLabel *status;

private slots:
    void install();
    bool moveFiles(bool dir, QString origin, QString target);
};

#endif
