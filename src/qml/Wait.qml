import QtQuick 2.2

Item {
    id: wait

    Rectangle {
        id: waveLayer
        width: wave.width
        height: wave.height+8
        AnimatedImage {
            id: wave
            source: "qrc:/images/pig/wave.gif"
        }
        anchors.horizontalCenter: msg.horizontalCenter
        anchors.verticalCenter: msg.verticalCenter
        anchors.verticalCenterOffset: -30
    }
    Text {
        id: msg
        text: "PLEASE WAIT"
        color: { if (waveLayer.visible) "white"; else Qt.rgba(0.1, 0.1, 0.1, 0.2) }
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: 30
        anchors.centerIn: parent
    }
}
