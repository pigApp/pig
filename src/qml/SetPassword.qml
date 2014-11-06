import QtQuick 2.3

Item {
    id: setPassword

    Rectangle {
        id: layer
        width: parent.width
        height: parent.height/4.32
        color: "black"
        anchors.centerIn: parent
        Text {
            id: label
            text: "INTRO NEW PASSWORD"
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
                    label.text = "INTRO NEW PASSWORD"
                else
                    label.text = ""
            }
            onCursorVisibleChanged: { if (input.cursorVisible) input.cursorVisible = false }
            onAccepted: {
                if (input.text !== "") {
                    root.password_handle_qml_signal(input.text, true)
                    input.visible = false
                    input.enabled = false
                }
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Escape) {
                    screen.state = "hideSetPassword"
                    event.accepted = true
                } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                    root.quit_qml_signal()
                    event.accepted = true;
                }
            }
        }
        Timer {
            id: hide
            running: false
            repeat: false
            interval: 3000
            onTriggered: { screen.state = "hideSetPassword" }
        }
    }

    Connections {
        target: cppSignals
        onSuccess_password_signal: {
            label.text = ""
            screen.state = "hideSetPassword"
        }
        onFail_password_signal: {
            label.text = "FAIL. TRY LATER"
            hide.start()
        }
    }

    Component.onCompleted: { input.forceActiveFocus() }
}
// Tabs hechos.
