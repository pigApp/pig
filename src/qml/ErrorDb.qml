import QtQuick 2.2

Item {
    id: errorDatabaseMessage

    Rectangle {
        id: layer
        width: screen.width
        height: 200
        color: "black"
        anchors.centerIn: parent
        Column {
            id: errorColumn
            spacing: 10
            anchors.centerIn: parent
            Text {
                id: errorInformationLabel
                text: "DATABASE IS CORRUPTED OR NOT EXIST"
                color: "white"
                font.family: pigFont.name
                font.pixelSize: 30
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: getDbInformationLabel
                text: "GET LATEST DATABASE FROM https://sourceforge/pigApp/db" // TODO: Url real db.
                color: Qt.rgba(0.1, 0.1, 0.1, 0.5)
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: errorDatabaseMessage.forceActiveFocus()
}
