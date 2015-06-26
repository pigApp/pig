#include "pig.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PIG p;
    p.showFullScreen();

<<<<<<< HEAD
    view->rootContext()->setContextProperty("cpp", &pig);
    view->setSource(QUrl("qrc:/QML/src/qml/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setIcon(QIcon("/resources/images/global/icon.jpg"));

    pig.set_root_object(view->rootObject());
    pig.password(true, "", false, false);

    view->showFullScreen();

    return app.exec();
=======
    return a.exec();
>>>>>>> port
}
