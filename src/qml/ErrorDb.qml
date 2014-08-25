import QtQuick 2.2

Item {
    id: errorDatabaseMessage

    Rectangle {
        id: layer
        width: parent.width
        height: parent.height/5.4
        color: "black"
        anchors.centerIn: parent
        Column {
            id: errorColumn
            spacing: parent.width/192
            anchors.centerIn: parent
            Text {
                id: errorInformationLabel
                text: "DATABASE IS CORRUPTED OR NOT EXIST"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: getDbInformationLabel
                text: "GET LATEST DATABASE FROM https://sourceforge/pigApp/db" // TODO: Url real db.
                color: Qt.rgba(0.1, 0.1, 0.1, 1)
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: errorDatabaseMessage.forceActiveFocus()
}
