import QtQuick 2.4

Item {
    id: welcome

    Text {
        id: label
        text: "CTRL H HELP"
        color: "darkgray"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        opacity: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    NumberAnimation { target: label; running: root.init; properties: "opacity"
        ; to: 1.0; duration: 3200; easing.type: Easing.InOutQuart }
}
// Tabs hechos.
