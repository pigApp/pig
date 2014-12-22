import QtQuick 2.3

Rectangle {
    id: errorDatabase
    color: "black"
    anchors.fill: parent

    Column {
        spacing: screen.height/108
        anchors.centerIn: parent
        Text {
            id: errorLabel
            text: "DATABASE CORRUPTED"
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: getLabel
            text: "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
                  "GET LATEST DATABASE &nbsp; <a href=\'http://google.com\'>DL.BINTRAY.COM/DB <font color='#ffffff'>➟</font></a>"
            color: "white"
            font.family: globalFont.name
            font.bold: true
            font.pixelSize: screen.height/54
            textFormat: Text.RichText
            onLinkActivated: Qt.openUrlExternally(link)
            anchors.horizontalCenter: errorLabel.horizontalCenter
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: errorDatabase.forceActiveFocus()
}
