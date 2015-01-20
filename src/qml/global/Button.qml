import QtQuick 2.4

Rectangle {
    id: button
    width: label.contentWidth
    height: label.contentHeight/1.6
    color: "transparent"

    property alias label: label.text
    property alias labelColor: label.color
    property alias labelBold: label.font.bold
    property alias labelLeftMargin: button.leftMargin
    property alias gridLayerVisible: gridLayer.visible

    property int leftMargin: 0
    property bool lockColor

    signal clicked()

    Image {
        id: gridLayer
        width: parent.width
        height: parent.height
        sourceSize.width: 455
        sourceSize.height: 50
        source: { if (gridLayer.visible) "qrc:/img-grid-small"; else "" }
        visible: false
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: label
        color: "white"
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.left: parent.left
        anchors.leftMargin: leftMargin
        anchors.verticalCenter: parent.verticalCenter
        ColorAnimation on color { id: in_label; running: false; to: "black"; duration: 100 }
        ColorAnimation on color { id: out_label; running: false; to: "white"; duration: 100 }
        MouseArea {
            hoverEnabled: true
            onEntered: {
                if (!lockColor) {
                    out_label.running = false
                    in_label.running = true
                }
            }
            onHoveredChanged: {
                if (!lockColor) {
                    in_label.running = false
                    out_label.running = true
                }
            }
            onClicked: button.clicked()
            anchors.fill: parent
        }
    }
}
// Tabs hechos.
