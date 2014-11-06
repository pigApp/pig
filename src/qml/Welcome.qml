import QtQuick 2.3

Item {
    id: welcome
    anchors.fill: parent

    Column {
        id: welcomeRow
        spacing: -parent.height/36
        anchors.centerIn: parent
        Text {
            id: welcomeLabel
            text: "WELCOME TO PIG"
            color: "black"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/10
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id: welcomeHelpLabel
            text: "CTRL H TO HELP"
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/23
            anchors.horizontalCenter: welcomeLabel.horizontalCenter
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
            screen.state = "showHelp"
            event.accepted = true
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quit_qml_signal()
            event.accepted = true
        }
    }

    Component.onCompleted: { welcome.forceActiveFocus() }
}
// Tabs hechos.
