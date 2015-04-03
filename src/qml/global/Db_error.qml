import QtQuick 2.4

Rectangle {
    id: db_error
    color: "red"

    Column {
        spacing: -parent.height/108
        anchors.centerIn: parent
        Row {
            id: rowStatus
            spacing: screen.width/192
            Text {
                id: label
                text: "FAIL"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelInformation
                text: "DATABASE ERROR"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Text {
            id: labelLink
            text: "<style type='text/css'>a:link{color:#ffffff;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>GET DATABASE</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
            anchors.horizontalCenter: rowStatus.horizontalCenter
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q)
            && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    Component.onCompleted: forceActiveFocus()
}
// Tabs hechos.
