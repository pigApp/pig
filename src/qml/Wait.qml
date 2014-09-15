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
        color: "black"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: parent.height/23
        visible: { root.networkError }
        anchors.horizontalCenter: networkIcon.horizontalCenter
        anchors.top: networkIcon.bottom
        anchors.topMargin: -parent.height/14.42
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
