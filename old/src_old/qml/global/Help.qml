import QtQuick 2.4

Item {
    id: help

    Column {
        id: columnHelp
        spacing: -screen.height/108
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -screen.height/108
        Row {
            spacing: screen.width/25.26
            Text {
                id: labelBackKey
                text: "ESC"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelback
                text: "BACK"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Row {
            spacing: screen.width/128
            Text {
                id: labelQuitKey
                text: "CTRL Q"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelQuit
                text: "QUIT"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Text {
            id: labelLink
            width: contentWidth
            height: contentHeight-(screen.height/108)
            text: "<style type='text/css'>a:link{color:#FFFFFF;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>MORE</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    Keys.onPressed: {
        screen.state = "hide_help"
        event.accepted = true
    }

    Component.onCompleted: help.forceActiveFocus()
}
// Tabs hechos.
