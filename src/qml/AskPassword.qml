import QtQuick 2.2

Item {
    id: askPassword

    Rectangle {
        id: layer
        width: screen.width
        height: 35
        color: Qt.rgba(0, 0, 0, 0.5)
        radius: 2
        anchors.centerIn: parent
        Text {
            id: label
            text: "YOUR PASSWORD"
            color: "white"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 30
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
        }
        TextInput {
            id: input
            color: "white"
            font.family: pigFont.name
            font.pixelSize: 25
            focus: true
            maximumLength: 16
            echoMode: TextInput.Password
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -1.5
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
                if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
                    root.quitSIGNAL_QML()
                    event.accepted = true;
                }
            }
        }
    }
}
// Espacios hechos.
