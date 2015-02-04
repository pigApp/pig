import QtQuick 2.4

Rectangle {
    id: button
    width: label.contentWidth
    height: label.contentHeight
    color: "transparent"

    property alias label: label.text
    property alias labelColor: label.color
    property alias labelLeftMargin: button.margin
    property alias layerGridVisible: layerGrid.visible

    property int margin: 0
    property bool lockColor

    signal clicked()

    Image {
        id: layerGrid
        width: parent.width
        height: parent.height
        sourceSize.width: layerGrid.width
        sourceSize.height: layerGrid.height
        source: { if (layerGrid.visible) "qrc:/img-grid-small"; else "" }
        visible: false
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: label
        width: label.contentWidth
        height: label.contentHeight
        color: "white"
        font.family: fontGlobal.name
        //font.bold: true
        font.pixelSize: screen.height/10 //23
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.verticalCenter: parent.verticalCenter
        ColorAnimation on color { id: inLabel; running: false; to: "black"; duration: 100 }
        ColorAnimation on color { id: outLabel; running: false; to: "white"; duration: 100 }
        MouseArea {
            hoverEnabled: true
            onEntered: {
                if (!lockColor) {
                    outLabel.running = false
                    inLabel.running = true
                }
            }
            onHoveredChanged: {
                if (!lockColor) {
                    inLabel.running = false
                    outLabel.running = true
                }
            }
            onClicked: button.clicked()
            anchors.fill: parent
            anchors.topMargin: screen.height/108
            anchors.bottomMargin: screen.height/108
        }
    }
}
// Tabs hechos.
