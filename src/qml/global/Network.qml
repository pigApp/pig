import QtQuick 2.4

Item {
    id: network

    Image {
        id: icon
        width: screen.width/30
        height: screen.height/25.11
        sourceSize.width: width
        sourceSize.height: height
        source:  { if (!root.network_err) "qrc:/img-network"; else "qrc:/img-network_err" }
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/108
        onSourceChanged: { if (root.network_err) delayBack.start() }
    }

    Timer {
        id: delayBack
        running: false
        repeat: false
        interval: 5000
        onTriggered: {
            root.network_err = false
            screen.state = "show_finder"
        }
    }
}
// Tabs hechos.
