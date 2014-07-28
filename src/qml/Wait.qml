import QtQuick 2.2

Item {
    id: wait

    Rectangle {
        id: waveLayer
        width: wave.width
        height: wave.height
        AnimatedImage {
            id: wave
            source: "qrc:/images/pig/wave.gif"
        }
        anchors.horizontalCenter: msg.horizontalCenter
        anchors.verticalCenter: msg.verticalCenter
    }
    Text {
        id: msg
        text: "PLEASE WAIT"
        color: "white"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: 30
        anchors.centerIn: parent
    }
}
