import QtQuick 2.3

Item {
    id: input

    Rectangle {
        id: layer
        width: screen.width/3.95
        height: screen.height/21.6
        color: Qt.rgba(0, 0, 0, 0.1)
    }
    Text {
        id: label
        text: "SEARCH"
        color: Qt.rgba(1, 1, 1, 0.3)
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/48
        anchors.left: layer.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: layer.verticalCenter
    }
    TextInput {
        id: user
        text: root.inputUser
        color: "black"
        font.family: globalFont.name
        font.capitalization: Font.AllUppercase
        font.bold: true
        font.pixelSize: screen.height/48
        maximumLength: 26
        cursorVisible: false
        anchors.left: layer.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: layer.verticalCenter
        onCursorVisibleChanged: { if (user.cursorVisible) user.cursorVisible = false }
        onCursorPositionChanged: {
            if (dbNullLabel.visible)
                dbNullLabel.visible = false
            if (user.text === "")
                label.text = "SEARCH"
            else
                label.text = ""
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
        color: Qt.rgba(1, 1, 1, 0.3)
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/48
        visible: false
        anchors.left: layer.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: layer.verticalCenter
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
