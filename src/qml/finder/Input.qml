import QtQuick 2.4

Item {
    id: input

    Image {
        id: gridLayer
        width: screen.width/4.22
        height: screen.height/21.6
        sourceSize.width: 455
        sourceSize.height: 50
        source: "qrc:/img-grid-small"
    }

    Text {
        id: label
        text: "SEARCH"
        color: Qt.rgba(1, 1, 1, 0.7)
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/48
        anchors.left: gridLayer.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: gridLayer.verticalCenter
    }
    TextInput {
        id: user
        text: root.inputUser
        color: "black"
        font.family: globalFont.name
        font.capitalization: Font.AllUppercase
        font.bold: true
        font.pixelSize: screen.height/48
        maximumLength: 24
        cursorVisible: false
        anchors.left: gridLayer.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: gridLayer.verticalCenter
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
        color: "black"
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/48
        visible: false
        anchors.left: gridLayer.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: gridLayer.verticalCenter
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
