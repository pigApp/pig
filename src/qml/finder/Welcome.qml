import QtQuick 2.4

Item {
    id: welcome

    Text {
        id: label
        text: "WELCOME &nbsp; <font color='#161616'>CTRL H TO HELP</font></a>"
        color: "white"
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        opacity: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    NumberAnimation { target: label; running: root.init; properties: "opacity"; to: 1.0; duration: 1400; easing.type: Easing.InOutQuart }
}
