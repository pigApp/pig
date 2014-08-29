import QtQuick 2.2

Item {
    id: setPassword

    Rectangle {
        id: layer
        width: parent.width
        height: parent.height/30.85
        color: Qt.rgba(0, 0, 0, 0.5)
        radius: 2
        anchors.centerIn: parent
        Text {
            id: label
            text: "NEW PASSWORD"
            anchors.left: parent.left
            anchors.leftMargin: 0
            color: "white"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/36
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
        }
        TextInput {
            id: input
            color: "white"
            font.family: pigFont.name
            font.pixelSize: screen.height/43.2
            maximumLength: 16
            echoMode: TextInput.Password
            visible: false
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -screen.height/720
            onCursorPositionChanged: { label.text = ""; input.visible = true }
            onAccepted: {
                if (input.text !== '') {
                    root.passwordHandleSIGNAL_QML(input.text, false, true)
                    input.visible = false
                    input.enabled = false
                    if (root.ok_password) {
                        label.text = ""
                        screen.state = "cleanUp"
                    } else {
                        label.text = "FAIL. TRY LATER"
                        back.start()
                    }
                }
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Escape) {
                    screen.state = "cleanUp"
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
            interval: 2000
            onTriggered: { screen.state = "cleanUp" }
        }
    }

    Component.onCompleted: { input.forceActiveFocus() }
}
// Espacios hechos.
