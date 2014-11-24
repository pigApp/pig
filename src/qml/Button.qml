import QtQuick 2.3

Rectangle {
    id: button
    color: "transparent"

    property bool enter
    property alias label: label.text
    property alias labelColor: label.color
    property alias labelSize: label.font.pixelSize
    property alias labelBold: label.font.bold
    property color labelInColor
    property color labelOutColor

    signal clicked()

    Text {
        id: label
        font.family: pigFont.name
        anchors.verticalCenter: parent.verticalCenter
        ColorAnimation on color { id: inColor; running: false; to: labelInColor; duration: 200 }
        ColorAnimation on color { id: outColor; running: false; to: labelOutColor; duration: 300 }
    }
    MouseArea {
        hoverEnabled: true
        onEntered: { enter = true }
        onHoveredChanged: { enter = false }
        onClicked: button.clicked()
        anchors.fill: parent
    }
    onEnterChanged: {
        if (enter) {
            inColor.start()
        } else {
            inColor.stop()
            outColor.start()
        }
    }
}
// Tabs hechos.
