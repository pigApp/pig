import QtQuick 2.3

Item {
    id: network

    Image {
        id: icon
        width: parent.width/30
        height: parent.height/16.87
        sourceSize.width: parent.width/30
        sourceSize.height: parent.height/16.87
        source:  {
            if (root.downloadNetwork)
                "/resources/images/network/icon_download.svg"
            else if (root.errorNetwork)
                "/resources/images/network/icon_ERROR.svg"
            else
                "/resources/images/network/icon_default.svg"
        }
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
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
            root_loader_B.source = "Finder.qml"
            root.errorNetwork = false
            root_loader_A.source = ""
        }
    }
}
