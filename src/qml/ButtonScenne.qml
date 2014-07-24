import QtQuick 2.2
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonScenne
    width: 100
    height: 80
    color: "transparent"

    property bool enter
    property string quality
    property string magnetUrl
    property string scenne

    signal clicked()
    onClicked: { onShowPlayerLayer = true; root.torrentHandle(magnetUrl, scenne, false) }
    
    RectangularGlow {
        id: effect
        color: "white"
        glowRadius: 1
        spread: 0
        cornerRadius: 90
        opacity: 0
        anchors.fill: icon
    }
    Image {
        id: icon
        source: { if (quality == '1080p') "qrc:/images/output/1080.png"; else "qrc:/images/output/720.png" }
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: { enter = true }
        onHoveredChanged: { enter = false }
        onClicked: { buttonScenne.clicked() }
        anchors.fill: parent
    }
    onEnterChanged: {
        if (enter) {
            inEffect.start()
        } else {
            inEffect.stop()
            outEffect.start()
        }
    }
    NumberAnimation { id: inEffect; target: effect; properties: "opacity"; to: 0.2; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outEffect; target: effect; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
}
// Espacios hechos.
