import QtQuick 2.4

Rectangle {
    id: errorDatabase
    color: "black"

    Column {
        spacing: -parent.height/216
        anchors.centerIn: parent
        Text {
            id: errorLabel
            text: "DATABASE ERROR"
            color: "red"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Row {
            spacing: screen.width/192
            anchors.horizontalCenter: errorLabel.horizontalCenter
            Text {
                id: getLabel
                text: "GET LATEST DATABASE"
                color: "white"
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/54
            }
            Text {
                id: linkLabel
                text: "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
                      "<a href=\'http://google.com\'>DL.BINTRAY.COM/DB <font color='#ffffff'>&nbsp;&nbsp;➟</font></a>"
                textFormat: Text.RichText
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/54
                onLinkActivated: Qt.openUrlExternally(link)
                anchors.verticalCenter: getLabel.verticalCenter
                anchors.verticalCenterOffset: screen.height/635
            }
        }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: errorDatabase.forceActiveFocus()
}
// Tabs hechos.
