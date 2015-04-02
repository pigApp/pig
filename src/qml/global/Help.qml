import QtQuick 2.4

Item {
    id: help

    Column {
        id: columnHelp
        spacing: -screen.height/108
        anchors.centerIn: parent
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
        Row {
            spacing: screen.width/128
            Text {
                id: labelHelpKey
                text: "CTRL H"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelHelp
                text: "HELP"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Row {
            spacing: screen.width/96
            Text {
                id: labelSettingKey
                text: "CTRL S"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelSetting
                text: "SETTING"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
    }
    Text {
        id: labelTroubleshooting
        text: "<style type='text/css'>a:link{color:#ffffff;font-style:normal;text-decoration:none}</style>"+
              "<a href=\'http://google.com\'>TROUBLESHOOTING</font> ➟</a>"
        textFormat: Text.RichText
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.top: columnHelp.bottom
        anchors.topMargin: -screen.height/108
        anchors.left: columnHelp.left
        onLinkActivated: Qt.openUrlExternally(link)
    }

    Keys.onPressed: {
        screen.state = "hide_help"
        event.accepted = true
    }

    Component.onCompleted: help.forceActiveFocus()
}
// Tabs hechos.
