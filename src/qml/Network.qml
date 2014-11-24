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
        font.pixelSize: screen.height/54
        visible: root.errorNetwork
        anchors.centerIn: parent
        anchors.verticalCenterOffset: screen.height/8.3
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
