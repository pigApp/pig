import QtQuick 2.3

Item {
    id: network

    Image {
        id: icon
        width: parent.width/5.36
        height: parent.height/3.01
        source: "/resources/images/network/icon.png"
        sourceSize.width: 358
        sourceSize.height: 358
        anchors.centerIn: parent
    }
    Text {
        id: errorLabel
        text: "NETWORK ERROR"
        color: "black"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: parent.height/23
        visible: root.errorNetwork
        anchors.horizontalCenter: icon.horizontalCenter
        anchors.top: icon.bottom
        anchors.topMargin: -parent.height/14.42
        onVisibleChanged: {
            if (root.errorNetwork) {
                icon.source = "/resources/images/network/icon_ERROR.png"
                backDelay.start()
            }
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
