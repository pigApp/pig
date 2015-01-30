import QtQuick 2.4

Item {
    id: input

    Image {
        id: layerGrid
        width: parent.width
        height: parent.height
        sourceSize.width: layerGrid.width
        sourceSize.height: layerGrid.height
        source: "qrc:/img-grid-small"
    }

    TextInput {
        id: userInput
        width: parent.width-(screen.width/76.8)
        text: root.userInput
        color: "white"
        font.family: fontGlobal.name
        font.capitalization: Font.AllUppercase
        font.bold: true
        font.pixelSize: screen.height/23
        wrapMode: TextInput.WordWrap
        cursorVisible: false
        anchors.left: layerGrid.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: layerGrid.verticalCenter
        onCursorPositionChanged: {
            if (icon.source !== "qrc:/img-find")
                icon.source = "qrc:/img-find"
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
    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: icon.width
        sourceSize.height: icon.height
        source: "qrc:/img-find"
        visible: false
        anchors.left: layerGrid.right
        anchors.leftMargin: screen.width/384
        anchors.verticalCenter: layerGrid.verticalCenter
    }

    onXChanged: {
        if (input.x > (-screen.width/4.25))
            icon.visible = true
        else
            icon.visible = false
    }
    onEnabledChanged: {
        userInput.forceActiveFocus()
        userInput.cursorVisible = false
    }

    Connections {
        target: cpp
        onSig_ret_db: {
            if ((n_films === 0) && (!loader_filter.active))
                icon.source = "qrc:/img-null"
            else
                icon.source = "qrc:/img-find"
        }
    }
}
// Tabs hechos.
