import QtQuick 2.4

Item {
    id: welcome

    Text {
        id: label
        text: "WELCOME &nbsp; <font color='#000000'>CTRL H TO HELP</font>"
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        opacity: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    NumberAnimation { target: label; running: root.onInit; properties: "opacity"; to: 1.0; duration: 1400; easing.type: Easing.InOutQuart }
}
// Tabs hechos.
