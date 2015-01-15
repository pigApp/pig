import QtQuick 2.4

Item {
    id: password

    Rectangle {
        id: layer
        color: "black"
        anchors.fill: parent
        Row {
            spacing: parent.width/192
            anchors.centerIn: parent
            Text {
                id: failLabel
                text: "FAIL"
                color: "red"
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/23
                visible: false
                anchors.verticalCenter: label.verticalCenter
                anchors.verticalCenterOffset: screen.height/540
            }
            Text {
                id: label
                text: "INTRO PASSWORD"
                color: "white"
                font.family: globalFont.name
                font.pixelSize: screen.height/23
            }
        }
        TextInput {
            id: user
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/43.2
            maximumLength: 16
            echoMode: TextInput.Password
            cursorVisible: false
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -screen.height/720
            onCursorVisibleChanged: { if (user.cursorVisible) user.cursorVisible = false }
            onCursorPositionChanged: {
                user.visible = true
                if (user.text === "")
                    label.text = "INTRO PASSWORD"
                else
                    label.text = ""
            }
            onAccepted: {
                if (user.text !== "") {
                    user.visible = false
                    if (root.askPassword) {
                        root.signal_qml_password_handler(user.text, false, true, false)
                    } else {
                        root.signal_qml_password_handler(user.text, false, false, true)
                        user.enabled = false
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
            onClicked: { user.focus = true }
            anchors.fill: parent
        }
    }

    Timer {
        id: hide
        running: false
        repeat: false
        interval: 5000
        onTriggered: { screen.state = "hide_password" }
    }

    Connections {
        target: cppSignals
        onSignal_ret_password: {
            if (success) {
                screen.state = "hide_password"
            } else {
                if (root.askPassword) {
                    label.text = "WRONG PASSWORD"
                } else {
                    failLabel.visible = true
                    label.text = "CHECK PERMISSIONS"
                    hide.start()
                }
            }
        }
    }

    Component.onCompleted: user.forceActiveFocus()
}
// Tabs hechos.
