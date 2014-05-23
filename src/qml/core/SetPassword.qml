import QtQuick 2.1

Item {
    id: setPassword

    Rectangle {
        id: layer
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.fill: parent
    }

    Column {
        id: formColumn
        spacing: 10
        anchors.centerIn: parent
        Row {
            spacing: 15
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
                color: Qt.rgba(0.8, 0.8, 0.8, 1)
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
                focus: true  // Falta quitar el foco a Finder, por que si apreto Enter sin hacer foco aca(input) Finder carga las peliculas.
                maximumLength: 16
                echoMode: TextInput.Password
                onAccepted: {
                    if (input.text !== '') {
                        root.passManager(input.text, false, true)
                        formColumn.visible = false
                        formColumn.enabled = false
                    }
                }
            }
        }
        Text {
            id: whatText
            text: "If set a password will be you requested when starting PIG.<br>Is recommended to use it if you share a computer with someone underage.<br>It is a basic measure of security. Is not invulnerable. Be careful."
            color: Qt.rgba(0.8, 0.8, 0.8, 1)
            font.family: pigFont.name
            font.italic: true
            font.pixelSize: 15
            opacity: 0
        }
    }

    Text {
        id: ok_fail_Label
        text: { if (root.okPass) "PASSWORD OK.<br>ESC TO BACK"; else "PASSWORD FAIL.<br>TRY LATER.<br>ESC TO BACK" }
        color: "white"
        font.family: pigFont.name
        font.pixelSize: 25
        visible: root.okPass || root.failPass && !root.requirePass
        anchors.centerIn: parent
    }
}
// Espacios hechos.
