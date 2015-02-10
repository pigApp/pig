import QtQuick 2.4

Rectangle {
    id: input
    color: "white"

    TextInput {
        id: userInput
        width: parent.width-(screen.width/54.85)
        text: root.userInput
        color: "black"
        font.family: fontGlobal.name
        font.capitalization: Font.AllUppercase
        font.bold: true
        font.pixelSize: screen.height/23
        wrapMode: TextInput.NoWrap
        persistentSelection: true
        selectByMouse: true
        selectedTextColor: "white"
        selectionColor: "gray"
        cursorVisible: false
        anchors.left: parent.left
        anchors.leftMargin: screen.width/120
        anchors.verticalCenter: parent.verticalCenter
        onCursorPositionChanged: {
            if (finderHandler.inputEffectColor === "red")
                finderHandler.inputEffectColor = "black"
            if (text !== "")
                cursorVisible = true
            else
                cursorVisible = false
        }
        onAccepted: {
            root.userInput = text
            cpp.find(root.userInput, root.pornstar, root.category, root.quality, root.full)
        }
    }

    onEnabledChanged: {
        userInput.forceActiveFocus()
        userInput.cursorVisible = false
    }

    Connections {
        target: cpp
        onSig_ret_db: { if ((n_films === 0) && (loader_filter.source == "")) finderHandler.inputEffectColor = "red" }
    }
}
// Tabs hechos.
