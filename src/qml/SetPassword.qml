import QtQuick 2.3

Item {
    id: setPassword

    Rectangle {
        id: layer
        color: "black"
        anchors.fill: parent
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
            }
            onCursorVisibleChanged: { if (input.cursorVisible) input.cursorVisible = false }
            onAccepted: {
                if (input.text !== "") {
                    root.signal_qml_password_handler(input.text, false, false, true)
                    input.visible = false
                    input.enabled = false
                }
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Escape) {
                    screen.state = "hide_setPassword"
                    event.accepted = true
                } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                    root.signal_qml_quit()
                    event.accepted = true;
                }
            }
        }
        Timer {
            id: hide
            running: false
            repeat: false
            interval: 3000
            onTriggered: { screen.state = "hide_setPassword" }
        }
    }

    Connections {
        target: cppSignals
        onSignal_success_password: {
            label.text = ""
            screen.state = "hide_setPassword"
        }
        onSignal_fail_password: {
            label.color = Qt.rgba(0.1, 0.1, 0.1, 1)
            label.text = "FAIL CHECK PERMISSIONS"
            hide.start()
        }
    }

    Component.onCompleted: input.forceActiveFocus()
}
// Tabs hechos.
