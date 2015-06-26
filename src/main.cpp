#include "pig.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PIG p;
    p.showFullScreen();

    return a.exec();
}
