import QtQuick 2.3

Rectangle {
    id: errorDatabase
    color: "black"
    anchors.fill: parent

    Text {
        id: errorDatabaseLabel
        text: "DATABASE CORRUPTED"
        color: Qt.rgba(0.1, 0.1, 0.1, 1)
        font.family: pigFont.name
        font.pixelSize: screen.height/23
        anchors.centerIn: parent
    }
    Text {
        id: getDatabase
        text: "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
              "GET LATEST DATABASE &nbsp; <a href=\'http://google.com\'>DL.BINTRAY.COM/DB <font color='#ffffff'>➟</font></a>"
        color: "white"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        textFormat: Text.RichText
        onLinkActivated: Qt.openUrlExternally(link)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/54
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: errorDatabase.forceActiveFocus()
}
