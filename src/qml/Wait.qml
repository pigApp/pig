import QtQuick 2.2

Item {
    id: wait

    Image {
        id: networkIcon
        width: parent.width/5.36
        height: parent.height/3.01
        source: "qrc:/images/pig/network.png"
        sourceSize.width: 358
        sourceSize.height: 358
        anchors.centerIn: parent
        SequentialAnimation {
            running: true
            loops: Animation.Infinite
            PropertyAction { target: networkIcon; property: "opacity"; value: 1 }
            PauseAnimation { duration: 500 }
            PropertyAction { target: networkIcon; property: "opacity"; value: 0.93 }
            PauseAnimation { duration: 500 }
        }
    }
    Text {
        id: networkErrorLabel
        text: "NETWORK ERROR"
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: parent.height/36
        visible: { root.networkError }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: parent.width/384
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/8
        onVisibleChanged: { 
            if (root.networkError) {
                networkIcon.source = "qrc:/images/pig/networkError.png"
                backDelay.start()
            }
        }
    }
    Timer {
        id: backDelay
        running: false
        repeat: false
        interval: 5000
        onTriggered: { loader_finder_output.source = "Finder.qml"; root.networkError = false; loader.source = "" }
    }
}
