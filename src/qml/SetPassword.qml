import QtQuick 2.2

Item {
    id: setPassword

    Rectangle {
        id: layer
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.fill: parent
    }

    Column {
        id: formColumn
        spacing: 5
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: 70
        Row {
            spacing: 12
            Text {
                id: setLabel
                text: "SET PASSWORD"
                color: "white"
                font.family: pigFont.name
                font.pixelSize: 25
            }
            Text {
                id: whatLabel
                width: 140
                height: 26
                text: "what is this?"
                color: Qt.rgba(1, 1, 1, 0.5)
                font.family: pigFont.name
                font.pixelSize: 25
                MouseArea {
                    id: mousearea
                    anchors.fill: parent
                }
                states: State {
                    when: mousearea.pressed
                    PropertyChanges { target: whatText; opacity: 1 }
                }
            }
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
                maximumLength: 16
                echoMode: TextInput.Password
                onAccepted: {
                    if (input.text !== '') {
                        root.passwordHandle(input.text, false, true)
                        formColumn.visible = false
                        formColumn.enabled = false
                    }
                }
            }
        }
        Text {
            id: whatText
            text: "If set a password will be you requested when starting PIG.<br>Is recommended to use it if you share a computer with someone underage.<br>It is a basic measure of security. Is not invulnerable. Be careful."
            color: Qt.rgba(1, 1, 1, 0.7)
            font.family: pigLightFont.name
            font.italic: true
            font.pixelSize: 15
            opacity: 0
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
