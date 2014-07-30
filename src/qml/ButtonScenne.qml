import QtQuick 2.2

Rectangle {
    id: buttonScenne
    width: 100
    height: 80
    color: "transparent"

    property bool enter
    property string quality
    property string magnet
    property string scenne
    property int size: 100
    
    Image {
        id: icon
        width: size
        height: size
        source: { if (quality == '1080p') "qrc:/images/output/1080.png"; else if (quality == '720p') "qrc:/images/output/720.png"; else "qrc:/images/output/sd.png" }
        sourceSize.width: buttonScenne.size
        sourceSize.height: buttonScenne.size
        smooth: true
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: { enter = true }
        onHoveredChanged: { enter = false }
        onClicked: { onShowPlayerLayer = true; root.torrentHandleSIGNAL_QML(magnet, scenne, false) }
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
    NumberAnimation { id: inEffect; target: buttonScenne; properties: "size"; to: 110; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outEffect; target: buttonScenne; properties: "size"; to: 100; duration: 300; easing.type: Easing.InOutQuart }
}
// Espacios hechos.
