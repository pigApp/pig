#include <QQuickItem>
#include <QQmlContext>
#include <QQuickView>

//#include "socket/tcpSocket.h"

#include "pig.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView *view = new QQuickView(QUrl("qrc:/src/qml/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setIcon(QIcon("qrc:/images/icon.png"));

    PIG pig;
    pig.setRootObject(view->rootObject());
    pig.container = QWidget::createWindowContainer(view);
    pig.container->setFocusPolicy(Qt::TabFocus);
    pig.layout->addWidget(pig.container);

    view->rootContext()->setContextProperty("cppSignals", &pig);

    pig.window->showFullScreen();

    //QString serverT = "178.73.198.210";
    //QString urlT = "/torrent/32D5DF418A00AD359F71713484B595A852B719E2.torrent";
    //TcpSocket soc;
    //soc.serverTorrent = serverT;
    //soc.urlTorrent = urlT;
    //soc.doConnect();

    return app.exec();
}


