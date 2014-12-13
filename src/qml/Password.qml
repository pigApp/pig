import QtQuick 2.3

Item {
    id: password

    Rectangle {
        id: layer
        color: "black"
        anchors.fill: parent
        Text {
            id: label
            text: "INTRO PASSWORD"
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
            anchors.centerIn: parent
        }
        TextInput {
            id: inputUser
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/43.2
            maximumLength: 16
            echoMode: TextInput.Password
            cursorVisible: false
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -screen.height/720
            onCursorPositionChanged: {
                inputUser.visible = true
                label.color = "white"
                if (inputUser.text === "")
                    label.text = "INTRO PASSWORD"
                else
                    label.text = ""
            }
            onCursorVisibleChanged: { if (inputUser.cursorVisible) inputUser.cursorVisible = false }
            onAccepted: {
                if (inputUser.text !== "") {
                    inputUser.visible = false
                    if (root.askPassword) {
                        root.signal_qml_password_handler(inputUser.text, false, true, false)
                    } else {
                        root.signal_qml_password_handler(inputUser.text, false, false, true)
                        inputUser.enabled = false
                    }
                }
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Escape && !root.askPassword) {
                    screen.state = "hide_password"
                    event.accepted = true
                } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                    root.signal_qml_quit()
                    event.accepted = true;
                }
            }
        }
        MouseArea {
            onClicked: { inputUser.focus = true }
            anchors.fill: parent
        }
    }

    Timer {
        id: hide
        running: false
        repeat: false
        interval: 3000
        onTriggered: { screen.state = "hide_password" }
    }

    Connections {
        target: cppSignals
        onSignal_ret_password: {
            if (success) {
                screen.state = "hide_password"
            } else {
                label.color = Qt.rgba(0.1, 0.1, 0.1, 1)
                if (root.askPassword) {
                    label.text = "WRONG PASSWORD"
                } else {
                    label.text = "FAIL CHECK PERMISSIONS"
                    hide.start()
                }
            }
        }
    }

    Component.onCompleted: inputUser.forceActiveFocus()
}
// Tabs hechos.
