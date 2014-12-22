import QtQuick 2.3

Item {
    id: input

    Rectangle {
        id: layer
        color: Qt.rgba(0, 0, 0, 0.1)
        anchors.fill: { if (dbNullLabel.visible) dbNullLabel; else user }
        anchors.margins: { if (user.text !== "") -screen.width/384; else 0 }
    }
    TextInput {
        id: user
        color: "white"
        text: root.inputUser
        font.family: customFont.name
        font.capitalization: Font.AllUppercase
        font.letterSpacing: screen.width/480
        font.wordSpacing: -screen.width/384
        font.pixelSize: screen.height/23
        maximumLength: 35
        cursorVisible: false
        onCursorVisibleChanged: { if (user.cursorVisible) user.cursorVisible = false }
        onCursorPositionChanged: {
            if (dbNullLabel.visible)
                dbNullLabel.visible = false
            user.visible = true
        }
        onAccepted: {
            root.inputUser = user.text
            root.signal_qml_find(root.inputUser, root.pornstar, root.category, root.quality, root.full, 0, true)
        }
    }
    Text {
        id: dbNullLabel
        text: "NO RESULT"
        color: Qt.rgba(1, 1, 1, 0.1)
        font.family: customFont.name
        font.capitalization: Font.AllUppercase
        font.letterSpacing: screen.width/480
        font.wordSpacing: -screen.width/384
        font.pixelSize: screen.height/23
        visible: false
    }

    onEnabledChanged: user.forceActiveFocus()

    Connections {
        target: cppSignals
        onSignal_ret_db: {
            if (block_films === 0) {
                user.visible = false
                dbNullLabel.visible = true
            }
        }
    }
}
// Tabs hechos.
