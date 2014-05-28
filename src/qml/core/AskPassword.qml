import QtQuick 2.1

Item {
    id: askPassword
    z: 1
    Column {
        id: formColumn
        spacing: 5
        anchors.centerIn: parent
        Text {
            id: label
            text: { if (root.failPass) "WRONG PASSWORD"; else "PASSWORD" }
            color: Qt.rgba(0.2, 0.2, 0.2, 0.4)
            font.family: pigFont.name
            font.pixelSize: 25
        }
        Rectangle {
            id: inputLayer
            width: screen.width/5.48
            height: 35
            color: "black"
            TextInput {
                id: input
                width: screen.width/4.5
                color: "white"
                font.family: pigFont.name
                font.pixelSize: 25
                focus: true
                maximumLength: 16
                echoMode: TextInput.Password
                onCursorPositionChanged: { if (label.text == "WRONG PASSWORD") root.failPass = false }
                onAccepted: {
                    if (input.text !== '')
                        root.passManager(input.text, false, false)
                }
            }
        }
    }
}
// Espacios hechos.
