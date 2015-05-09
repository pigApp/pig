import QtQuick 2.4

Item {
    id: button
    width: label.contentWidth
    height: label.contentHeight

    property alias label: label.text
    property alias labelSize: label.font.pixelSize

    property string labelColor: "white"
    property string labelColorIn

    signal clicked()

    Text {
        id: label
        width: contentWidth
        height: contentHeight
        color: labelColor
        font.family: fontGlobal.name
        font.bold: true
        ColorAnimation on color { id: labelIn
            ; running: false; to: labelColorIn; duration: 100 }
        ColorAnimation on color { id: labelOut
            ; running: false; to: labelColor; duration: 100 }
        MouseArea {
            hoverEnabled: true
            anchors.topMargin: screen.height/83.07
            anchors.leftMargin: screen.width/960
            anchors.bottomMargin: screen.height/90
            anchors.fill: parent
            onEntered: {
                labelOut.running = false
                labelIn.running = true
            }
            onHoveredChanged: {
                labelIn.running = false
                labelOut.running = true
            }
            onClicked: button.clicked()
        }
    }
}
// Tabs hechos.
