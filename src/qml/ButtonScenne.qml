import QtQuick 2.2
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonScenne
    width: 100
    height: 80
    color: "transparent"

    property string quality
    property string magnetUrl
    property string scenne

    signal clicked()
    onClicked: { onShowPlayerLayer = true; root.torrentHandle(magnetUrl, scenne) }
    
    RectangularGlow {
        id: effect
        color: "white"
        glowRadius: 1
        spread: 0
        cornerRadius: 90
        opacity: 0.3
        visible: false
        anchors.fill: icon
    }
    Image {
        id: icon
        source: { if (quality == '1080p') "qrc:/images/1080.png"; else "qrc:/images/720.png" }
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: delayIn.restart()
        onHoveredChanged: delayOut.restart()
        onClicked: buttonScenne.clicked()
        anchors.fill: parent
    }
    Timer {
        id: delayIn
        running: false
        repeat: false
        interval: 25
        onTriggered: { effect.visible = true }
    }
    Timer {
        id: delayOut
        running: false
        repeat: false
        interval: 20
        onTriggered: { effect.visible = false }
    }
}
// Espacios hechos.
