import QtQuick 2.4

Item {
    id: shortcuts

    Column {
        id: columnKeys
        spacing: -screen.height/72
        anchors.left: parent.left
        anchors.leftMargin: screen.width/16
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: labelKeys
            text: "KEYS"
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
        Text {
            text: " "
            font.family: fontGlobal.name
            font.pixelSize: screen.height/58
        }
        Row {
            spacing: screen.width/128
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
    }
}
// Tabs hechos.
