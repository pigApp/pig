import QtQuick 2.3

Item {
    id: askPassword

    Rectangle {
        id: layer
        width: parent.width
        height: parent.height/4.32
        color: "black"
        anchors.centerIn: parent
        Text {
            id: label
            text: "INTRO PASSWORD"
            color: "white"
            font.family: pigFont.name
            font.pixelSize: screen.height/23
            anchors.centerIn: parent
        }
        TextInput {
            id: input
            color: "white"
            font.family: pigFont.name
            font.pixelSize: screen.height/43.2
            maximumLength: 16
            echoMode: TextInput.Password
            cursorVisible: false
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -screen.height/720
            onCursorPositionChanged: {
                if (input.text === "")
                    label.text = "INTRO PASSWORD"
                else
                    label.text = ""
                input.visible = true
            }
            onCursorVisibleChanged: { if (input.cursorVisible) input.cursorVisible = false }
            onAccepted: {
                if (input.text !== "")
                    root.signal_qml_password_handler(input.text, false, true, false)
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                    root.signal_qml_quit()
                    event.accepted = true;
                }
            }
        }
    }

    Connections {
        target: cppSignals
        onSignal_fail_password: {
            input.visible = false
            label.text = "WRONG PASSWORD"
        }
    }

    Component.onCompleted: input.forceActiveFocus()
}
// Tabs hechos.
