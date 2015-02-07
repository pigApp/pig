import QtQuick 2.4

Rectangle {
    id: help
    color: Qt.rgba(1, 1, 1, 0.5)

    Text {
        text: "FILTERS"
        color: "black"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.left: parent.left
        anchors.leftMargin: 700
        anchors.verticalCenter: parent.verticalCenter
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            screen.state = "hide_help"
            event.accepted = true;
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: help.forceActiveFocus()
}
// Tabs hechos.
