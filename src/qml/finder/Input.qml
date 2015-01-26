import QtQuick 2.4

Item {
    id: input

    Image {
        id: gridLayer
        width: parent.width
        height: parent.height
        sourceSize.width: gridLayer.width
        sourceSize.height: gridLayer.height
        source: "qrc:/img-grid-small"
    }

    TextInput {
        id: user
        width: parent.width-(screen.width/76.8)
        text: root.inputUser
        color: "white"
        font.family: globalFont.name
        font.capitalization: Font.AllUppercase
        font.bold: true
        font.pixelSize: screen.height/23
        wrapMode: TextInput.WordWrap
        cursorVisible: false
        anchors.left: gridLayer.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: gridLayer.verticalCenter
        onCursorPositionChanged: {
            if (icon.source !== "qrc:/img-find")
                icon.source = "qrc:/img-find"
            if (user.text !== "")
                user.cursorVisible = true
            else
                user.cursorVisible = false
        }
        onAccepted: {
            root.inputUser = user.text
            root.signal_qml_find(root.inputUser, root.pornstar, root.category, root.quality, root.full, 0, true)
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
        anchors.left: gridLayer.right
        anchors.leftMargin: screen.width/384
        anchors.verticalCenter: gridLayer.verticalCenter
    }

    onXChanged: {
        if (input.x > (-screen.width/4.25))
            icon.visible = true
        else
            icon.visible = false
    }
    onEnabledChanged: {
        user.forceActiveFocus()
        user.cursorVisible = false
    }

    Connections {
        target: cppSignals
        onSignal_ret_db: {
            if ((block_films === 0) && (!filter_loader.active))
                icon.source = "qrc:/img-null"
            else
                icon.source = "qrc:/img-find"
        }
    }
}
// Tabs hechos.
