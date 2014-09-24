#include "installer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Installer installer;
    installer.show();

    return a.exec();
}
