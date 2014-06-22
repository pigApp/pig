import QtQuick 2.2

Rectangle {
    id: button
    color: "transparent"

    property alias label: label.text
    property alias labelColor: label.color
    property alias labelSize: label.font.pixelSize
    property alias labelBold: label.font.bold
    property color labelInColor
    property color labelOutColor

    signal clicked()
    
    Text {
        id: label
        color: "white"
        font.family: pigFont.name
        font.bold: false
        font.pixelSize: 110
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: label.color = labelInColor
        onHoveredChanged: label.color = labelOutColor
        onClicked: button.clicked()
        anchors.fill: parent
    }
}
// Espacios hechos.
