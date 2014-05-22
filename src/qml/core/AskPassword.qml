import QtQuick 2.1

Item {
    id: askPassword
    z: 1
    Column {
        id: formColumn
        spacing: 10
        anchors.centerIn: parent
        Text {
            id: label
            text: { if (root.failPass) "BAD PASSWORD"; else "PASSWORD" }
            color: Qt.rgba(0.5, 0.5, 0.5, 0.7)
            font.family: pigFont.name
            font.pixelSize: 40
        }
        Rectangle {
            id: inputLayer
            width: screen.width/3.4
            height: 54
            color: "black"
            TextInput {
                id: input
                width: screen.width/3.4
                color: "white"
                font.family: pigFont.name
                font.pixelSize: 40
                focus: true
                maximumLength: 16
                echoMode: TextInput.Password
                onCursorPositionChanged: { if(label.text == "BAD PASSWORD") root.failPass = false }
                onAccepted: {
                    if (input.text !== '')
                        root.passManager(input.text, false, false)
                }
            }
        }
    }
}
// Espacios hechos.

