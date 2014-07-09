import QtQuick 2.2

Item {
    id: help

    Rectangle {
      id: backgroundLayer
      color: Qt.rgba(0.5, 0.5, 0.5, 0.7)
      anchors.fill: parent
    }

    Rectangle {
        id: pigLayer
        width: screen.width/3.85
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        Column {
            x: 50
            y: 40
            spacing: 5
            Text {
                text: "PIG "+binaryVersion+"."+release+" DATABASE "+databaseVersion+" GPL 2.0"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "Is Free Software and free. Make for users to users.<br/>Not enterprises, not tracking. Not bullshits.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: 20
            }

            Text {
                text: "COLLABORATE"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "CODE &nbsp;<font color='#aeaeae'>GITHUB.COM/PIGAPP</font><br/>TORRENTS &nbsp;<font color='#aeaeae'>GITHUB.COM/PIGAPP/TORRENT</font><br/>TRANSLATE &nbsp;<font color='#aeaeae'>GITHUB.COM/PIGAPP/TRANSLATE</font><br/>MONEY &nbsp;<font color='#aeaeae'>BITCOIN 32kp42jk2323po3423nh2n</font><br/>SHARE &nbsp;<font color='#aeaeae'>PIG.NET</font><br/><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 20
                textFormat: Text.RichText
            }
        }
    }
    Rectangle {
        id: shortCutsLayer
        width: screen.width/3.85
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: pigLayer.right
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        Column {
            x: 50
            y: 40
            spacing: 5
            Text {
                text: "SHORTCUTS"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "Is Free Software and free. Make for users to users.<br/>Not publisher, not tracking. Not bullshits.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: 20
            }

            Text {
                text: "GENERAL"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "ESC &nbsp;<font color='#aeaeae'>BACK</font><br/>CTRL P &nbsp;<font color='#aeaeae'>PASSWORD</font><br/>CTRL H &nbsp;<font color='#aeaeae'>HELP</font><br/>CTRL ESC &nbsp;<font color='#aeaeae'>CLOSE</font><br/><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 20
                textFormat: Text.RichText
            }

            Text {
                text: "MOVE"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "Use to move on the result of search."
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: 20
            }

            Text {
                text: "ARROWS &nbsp;<font color='#aeaeae'>DOWN UP &nbsp; - + FIVE MOVIES</font><br/>ARROWS &nbsp;<font color='#aeaeae'>LEFT RIGHT &nbsp; PREVIOUS NEXT MOVIE</font><br/><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 20
                textFormat: Text.RichText
            }

            Text {
                text: "PLAYER"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "SPACE &nbsp;<font color='#aeaeae'>PLAY PAUSE</font><br/>ARROWS &nbsp;<font color='#aeaeae'>DOWN UP &nbsp; VOLUME</font><br/>ARROWS &nbsp;<font color='#aeaeae'>LEFT RIGHT &nbsp; - + TEN SECONDS</font><br/><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 20
                textFormat: Text.RichText
            }
        }
    }
    Rectangle {
        id: recomendationsLayer
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: shortCutsLayer.right
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        Column {
            x: 50
            y: 40
            spacing: 5
            Text {
                text: "RECOMENDATIONS"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "Is Free Software and free. Make for users to users.<br/>Not publisher, not tracking. Not bullshits.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: 20
            }
            Text {
                text: "UPDATE"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 30
            }
            Text {
                text: "Accept updates. It's fast and generally is of<br/>database with new films."
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: 20
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape && !(event.modifiers & Qt.ControlModifier)) {
            screen.state = "cleanUp"
            event.accepted = true;
        } else if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
            root.quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: { help.forceActiveFocus() }
}
