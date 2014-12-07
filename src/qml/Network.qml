import QtQuick 2.3

Item {
    id: network

    Image {
        id: icon
        width: parent.width/30
        height: parent.height/25.35
        sourceSize.width: parent.width/30
        sourceSize.height: parent.height/25.35
        source:  {
            if (!root.errorNetwork)
                "/resources/images/network/network.svg"
            else
                "/resources/images/network/network_ERROR.svg"
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
