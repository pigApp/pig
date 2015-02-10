import QtQuick 2.4

Item {
    id: button
    width: label.contentWidth
    height: label.contentHeight

    property alias label: label.text
    property alias labelColor: label.color
    property alias labelBold: label.font.bold
    property alias labelSize: label.font.pixelSize

    property bool lockColor
    property int labelMargin: 0
    property int mouseAreaMargin: screen.height/36

    signal clicked()

    Text {
        id: label
        width: contentWidth
        height: contentHeight
        color: "white"
        font.family: fontGlobal.name
        font.bold: false
        font.pixelSize: screen.height/10
        anchors.left: parent.left
        anchors.leftMargin: labelMargin
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
            anchors.topMargin: mouseAreaMargin
            anchors.bottomMargin: mouseAreaMargin
        }
    }
}
// Tabs hechos.
