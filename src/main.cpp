#include <QQuickItem>
#include <QQmlContext>
#include <QQuickView>

#include "pig.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView *view = new QQuickView(QUrl("qrc:/src/qml/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setIcon(QIcon("qrc:/images/pig/icon.png"));

    PIG pig;
    pig.setRootObject(view->rootObject());
    pig.container = QWidget::createWindowContainer(view);
    pig.container->setFocusPolicy(Qt::TabFocus);
    pig.layout->addWidget(pig.container);

    view->rootContext()->setContextProperty("cppSignals", &pig);

    pig.showFullScreen();

    return app.exec();
}


