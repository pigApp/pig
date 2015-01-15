import QtQuick 2.4

Item {
    id: network

    Image {
        id: icon
        width: screen.width/30
        height: screen.height/25.11
        sourceSize.width: 64
        sourceSize.height: 43
        source:  {
            if (!root.errorNetwork)
                "qrc:/img-network"
            else
                "qrc:/img-network-err"
        }
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/108
        onSourceChanged: {
            if (root.errorNetwork)
                backDelay.start()
        }
    }

    Timer {
        id: backDelay
        running: false
        repeat: false
        interval: 5000
        onTriggered: {
            root.errorNetwork = false
            screen.state = "show_finder"
        }
    }
}
// Tabs hechos.
