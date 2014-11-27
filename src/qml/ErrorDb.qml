import QtQuick 2.3

Item {
    id: errorDatabaseMessage

    Rectangle {
        id: layer
        width: parent.width
        height: parent.height/4
        color: "black"
        anchors.centerIn: parent
        Column {
            id: errorColumn
            spacing: parent.height/216
            anchors.centerIn: parent
            Text {
                id: errorInformationLabel
                text: "DATABASE IS CORRUPTED"
                color: "white"
                font.family: pigFont.name
                font.pixelSize: screen.height/23
            }
            Text {
                id: getDbInformationLabel
                text:"<style type='text/css'> a:link{color:#808080;font-style:normal;text-decoration:none} </style>"+
                     "GET LATEST DATABASE &nbsp;<a href=\'http://google.com\'>DL.BINTRAY.COM/DB <font color='#ff0000'>➟<font/></a>" // TODO: Url real db.
                color: "gray"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
                anchors.horizontalCenter: errorInformationLabel.horizontalCenter
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: errorDatabaseMessage.forceActiveFocus()
}
