#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>

#include "pig.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<PIG>("signals", 1, 0, "SIGNALS");

    QQuickView *view = new QQuickView(QUrl("qrc:/src/qml/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setIcon(QIcon("qrc:/images/icon.png"));

    PIG pig;
    pig.setRootObject(view->rootObject());
    pig.container = QWidget::createWindowContainer(view);
    pig.container->setFocusPolicy(Qt::TabFocus);
    pig.layout->addWidget(pig.container);
    
    pig.window->showFullScreen();

    return app.exec();
}


