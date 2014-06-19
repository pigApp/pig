import QtQuick 2.2

Item {
    id: askPassword
    z: 1

    Rectangle {
        id: inputLayer
        width: screen.width/5.48
        height: 35
        color: "black"
        radius: 2
        anchors.centerIn: parent
        TextInput {
            id: input
            width: screen.width/4.5
            color: "white"
            font.family: pigFont.name
            font.pixelSize: 25
            focus: true
            maximumLength: 16
            echoMode: TextInput.Password
            onCursorPositionChanged: { labelLayer.visible = false }
            onAccepted: {
                if (input.text !== '') {
                    root.passwordHandle(input.text, false, false)
                    if (root.failPass) {
                        label.text = "WRONG PASSWORD"
                        labelLayer.visible = true
                    }
                }
            }
        }
    }
    Rectangle {
        id: labelLayer
        width: screen.width/5.48
        height: 35
        color: "black"
        radius: 2
        anchors.centerIn: parent
        Text {
            id: label
            text: "YOUR PASSWORD"
            color: "white"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 25
            anchors.centerIn: parent
        }
    }
}
// Espacios hechos.
