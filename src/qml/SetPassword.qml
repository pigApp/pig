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
                if (input.text !== '') {
                    root.passwordHandleSIGNAL_QML(input.text, false, true)
                    input.visible = false
                    input.enabled = false
                    if (root.ok_password) {
                        label.text = ""
                        screen.state = "hideSetPassword"
                    } else {
                        label.text = "FAIL. TRY LATER"
                        back.start()
                    }
                }
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Escape) {
                    screen.state = "hideSetPassword"
                    event.accepted = true
                } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                    root.quitSIGNAL_QML()
                    event.accepted = true;
                }
            }
        }
        Timer {
            id: back
            running: false
            repeat: false
            interval: 3000
            onTriggered: { screen.state = "hideSetPassword" }
        }
    }

    Component.onCompleted: { input.forceActiveFocus() }
}
// Espacios hechos.
