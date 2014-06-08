import QtQuick 2.2

Item {
    id: askPassword
    z: 1
    Column {
        id: formColumn
        spacing: 5
        anchors.centerIn: parent
        Text {
            id: label
            text: { if (root.failPass) "WRONG PASSWORD"; else "YOUR PASSWORD" }
            color: "black"
            font.family: pigLightFont.name
            font.pixelSize: 25
            anchors.horizontalCenter: parent.horizontalCenter
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
                font.family: pigLightFont.name
                font.pixelSize: 25
                focus: true
                maximumLength: 16
                echoMode: TextInput.Password
                onCursorPositionChanged: { if (label.text == "WRONG PASSWORD") root.failPass = false }
                onAccepted: {
                    if (input.text !== '')
                        root.passwordHandle(input.text, false, false)
                }
            }
        }
    }
}
// Espacios hechos.
