import QtQuick 2.2

Item {
    id: askPassword

    Rectangle {
        id: layer
        width: parent.width
        height: parent.height/30.85
        color: Qt.rgba(0, 0, 0, 0.5)
        radius: 2
        anchors.centerIn: parent
        Text {
            id: label
            text: "YOUR PASSWORD"
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
            focus: true
            maximumLength: 16
            echoMode: TextInput.Password
            visible: false
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -screen.height/720
            onCursorPositionChanged: { label.text = ""; input.visible = true }
            onAccepted: {
                if (input.text !== '') {
                    root.passwordHandleSIGNAL_QML(input.text, false, false)
                    if (root.fail_password) {
                        input.visible = false
                        label.text = "WRONG PASSWORD"
                    }
                }
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                    root.quitSIGNAL_QML()
                    event.accepted = true;
                }
            }
        }
    }
}
// Espacios hechos.
