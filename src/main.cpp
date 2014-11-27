#include "pig.h"

#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView *view = new QQuickView();

    PIG pig;

    view->rootContext()->setContextProperty("cppSignals", &pig);
    view->setSource(QUrl("qrc:/QML/src/qml/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setIcon(QIcon("/resources/images/general/icon.png"));

    pig.set_root_object(view->rootObject());
    pig.container = QWidget::createWindowContainer(view);
    pig.container->setFocusPolicy(Qt::TabFocus);
    pig.layout->addWidget(pig.container);
    pig.password_handler("", true, false, false);

    pig.showFullScreen();

    return app.exec();
}
