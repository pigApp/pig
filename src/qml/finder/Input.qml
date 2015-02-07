import QtQuick 2.4

Rectangle {
    id: input
    color: Qt.rgba(1, 1, 1, 0.5)

    TextInput {
        id: userInput
        width: parent.width-(screen.width/76.8)
        text: root.userInput
        color: "black"
        font.family: fontGlobal.name
        font.capitalization: Font.AllUppercase
        font.bold: true
        font.pixelSize: screen.height/23
        cursorVisible: false
        wrapMode: TextInput.NoWrap
        anchors.left: parent.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: parent.verticalCenter
        onCursorPositionChanged: {
            if (input.color == "#ff0000")
                input.color = Qt.rgba(1, 1, 1, 0.5)
            if (userInput.text !== "")
                userInput.cursorVisible = true
            else
                userInput.cursorVisible = false
        }
        onAccepted: {
            root.userInput = userInput.text
            cpp.find(root.userInput, root.pornstar, root.category, root.quality, root.full)
        }
    }

    onEnabledChanged: {
        userInput.forceActiveFocus()
        userInput.cursorVisible = false
    }

    Connections {
        target: cpp
        onSig_ret_db: { if ((n_films === 0) && (loader_filter.source == "")) input.color = "red" }
    }
}
// Tabs hechos.
