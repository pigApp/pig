#include "pig.h"

#include <QApplication>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQuickView *view = new QQuickView();
    PIG pig;

    view->rootContext()->setContextProperty("cpp", &pig);
    view->setSource(QUrl("qrc:/QML/src/qml/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setIcon(QIcon("/resources/images/global/icon.jpg"));

    pig.set_root_object(view->rootObject());
    pig.password_handler(true, "", false, false);

    view->showFullScreen();

    return app.exec();
}
