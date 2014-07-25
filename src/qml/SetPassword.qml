import QtQuick 2.2

Item {
    id: setPassword

    Rectangle {
        id: layer
        width: screen.width
        height: 35
        color: Qt.rgba(0, 0, 0, 0.5)
        radius: 2
        anchors.centerIn: parent
        Text {
            id: label
            text: "SET PASSWORD"
            anchors.left: parent.left
            anchors.leftMargin: 0
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
            maximumLength: 16
            echoMode: TextInput.Password
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -1.5
            onCursorPositionChanged: { label.text = ""; input.visible = true }
            onAccepted: {
                if (input.text !== '') {
                    root.passwordHandleSIGNAL_QML(input.text, false, true)
                    input.visible = false
                    input.enabled = false
                    if (root.ok_password)
                        label.text = "DONE"
                    else
                        label.text = "FAIL. TRY LATER"
                    back.start()
                }
            }
            Keys.onPressed: {
                if (event.key === Qt.Key_Escape && !(event.modifiers & Qt.ControlModifier)) {
                    screen.state = "cleanUp"
                    event.accepted = true
                } else if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
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
