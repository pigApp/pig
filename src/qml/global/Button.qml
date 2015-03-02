import QtQuick 2.4

Item {
    id: button
    width: label.contentWidth
    height: label.contentHeight

    property alias label: label.text

    property string iconSource: ""

    signal clicked()

    Image {
        id: icon
        width: button.width
        height: button.height
        sourceSize.width: width
        sourceSize.height: height
        source: iconSource
        anchors.centerIn: parent
    }
    Text {
        id: label
        width: contentWidth
        height: contentHeight
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        ColorAnimation on color { id: labelIn; running: false; to: "black"; duration: 100 }
        ColorAnimation on color { id: labelOut; running: false; to: "white"; duration: 100 }
        MouseArea {
            hoverEnabled: true
            anchors.topMargin: screen.height/83.07
            anchors.leftMargin: screen.width/960
            anchors.bottomMargin: screen.height/90
            anchors.fill: parent
            onEntered: {
                if (iconSource === "") {
                    labelOut.running = false
                    labelIn.running = true
                }
            }
            onHoveredChanged: {
                if (iconSource === "") {
                    labelIn.running = false
                    labelOut.running = true
                }
            }
            onClicked: button.clicked()
        }
    }
}
// Tabs hechos.
