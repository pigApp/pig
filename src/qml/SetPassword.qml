import QtQuick 2.2

Item {
    id: setPassword

    Rectangle {
        id: layer
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.fill: parent
    }

    Rectangle {
        id: inputLayer
        width: screen.width/5.48
        height: 35
        radius: 2
        color: "black"
        anchors.centerIn: parent
        TextInput {
            id: input
            width: screen.width/4.5
            color: "white"
            font.family: pigFont.name
            font.pixelSize: 25
            maximumLength: 16
            echoMode: TextInput.Password
            onCursorPositionChanged: { label.visible = false }
            onAccepted: {
                if (input.text !== '') {
                    root.passwordHandle(input.text, false, true)
                    inputLayer.visible = false
                }
            }
        }
        Text {
            id: label
            text: "SET A PASSWORD"
            anchors.left: parent.left
            anchors.leftMargin: 0
            color: "white"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 25
            anchors.centerIn: parent
        }
    }
    Text {
        id: ok_fail_Label
        text: { if (root.okPass) "DONE. ESC TO BACK"; else "FAIL. TRY LATER. ESC TO BACK" }
        color: "white"
        font.family: pigFont.name
        font.pixelSize: 25
        visible: root.okPass || root.failPass 
        anchors.centerIn: parent
    }

    Component.onCompleted: { input.forceActiveFocus() }
}
// Espacios hechos.
