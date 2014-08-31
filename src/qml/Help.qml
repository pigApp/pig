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
        width: parent.width/4.10
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.topMargin: parent.height/108
        anchors.left: parent.left
        anchors.leftMargin: parent.width/192
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/108
        Column {
            x: parent.width/25
            y: parent.height/43
            spacing: parent.height/216
            Text {
                text: "PIG "+binaryVersion+"."+release+" DATABASE "+databaseVersion
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "Is Free Software and free. Made by users for users.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }

            Text {
                text: "COLLABORATE"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "<style type='text/css'> a:link{color:#757575;font-style:normal;text-decoration:none} </style>"+
                      "BUGS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/BUGS <font color='#ffffff'>➟<font/></a><br/>"+
                      "CODE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP <font color='#ffffff'>➟<font/></a><br/>"+
                      "TORRENTS &nbsp; <a href=\'http://clarin.com\'>GITHUB.COM/PIGAPP/TORRENT <font color='#ffffff'>➟<font/></a><br/>"+
                      "TRANSLATIONS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/TRANSLATE <font color='#ffffff'>➟<font/></a><br/>"+
                      "BTC &nbsp; <font color='#757575'>1LtU9rMsQ41rCqsJAvMtw89TA5XT2dW7f9</font><br/>"+
                      "SHARE &nbsp; <a href=\'http://google.com\'>PIG.NET <font color='#ffffff'>➟<font/></a><br/>"+
                      "CONTACT &nbsp; <font color='#757575'>PIG@MAIL.COM</font><br/><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                text: "LICENSE"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "GPL 2.0"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }
            Text {
                text: "<font color='#757575'>This program is distributed in the hope that it will be useful</font><br/>"+
                      "<font color='#757575'>but WITHOUT ANY WARRANTY; without even the implied warranty of</font><br/>"+
                      "<font color='#757575'>MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.</font><br/>"+
                      "<font color='#757575'>See the GNU General Public License for more details.</font>"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/74
            }
        }
    }
    Rectangle {
        id: shortcutsLayer
        width: parent.width/4.10
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.topMargin: parent.height/108
        anchors.left: pigLayer.right
        anchors.leftMargin: parent.width/192
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/108
        Column {
            x: parent.width/25
            y: parent.height/43
            spacing: parent.height/216
            Text {
                text: "SHORTCUTS"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "PIG is handled mostly with the keyboard.<br/>" 
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }

            Text {
                text: "GENERAL"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "ESC &nbsp; <font color='#757575'>BACK</font><br/>"+
                      "CTRL H &nbsp; <font color='#757575'>HELP</font><br/>"+
                      "CTRL P &nbsp; <font color='#757575'>PASSWORD</font><br/>"+
                      "CTRL Q &nbsp; <font color='#757575'>CLOSE</font><br/>"+
                      "ALT TAB &nbsp; <font color='#757575'>PIG DESKTOP</font><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
            }

            Text {
                text: "FINDER"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "INTRO &nbsp; <font color='#757575'>FIND</font><br/>"+
                      "UP DOWN &nbsp; <font color='#757575'>OFFSET FIVE MOVIES</font><br/>"+
                      "LEFT RIGHT &nbsp; <font color='#757575'>CHANGE MOVIE</font><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
            }

            Text {
                text: "PLAYER"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "SPACE &nbsp; <font color='#757575'>PAUSE</font><br/>"+
                      "CTRL L &nbsp; <font color='#757575'>LOOP</font><br/>"+
                      "UP DOWN &nbsp; <font color='#757575'>VOLUME</font><br/>"+
                      "LEFT RIGHT &nbsp; <font color='#757575'>OFFSET</font>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
            }
        }
    }
    Rectangle {
        id: recomendationsLayer
        width: parent.width/4.10
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.topMargin: parent.height/108
        anchors.left: shortcutsLayer.right
        anchors.leftMargin: parent.width/192
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/108
        Column {
            x: parent.width/25
            y: parent.height/43
            spacing: parent.height/216
            Text {
                text: "RECOMENDATIONS"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "Recommendations and misc.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }
            Text {
                text: "UPDATES"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "Accept them. Are fast and mostly of the<br/>"+
                      "database with new movies.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }
            Text {
                text: "RESOLUTIONS"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "⍿ &nbsp; <font color='#757575'>1920X1080</font><br/>"+
                      "⍿ &nbsp; <font color='#757575'>1440X975</font><br/>"+
                      "⍿ &nbsp; <font color='#757575'>1280X720</font><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
            }
            Text {
                text: "TRACKING"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "Those who create this software agree not to<br/>"+
                      "collect any user data. Note that this software<br/>"+
                      "does not use encrypted connections. So it<br/>"+
                      "does not ensure that third parties do not spy<br/>"+
                      "what you see.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }
            Text {
                text: "TAKE PART"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "If you use this software, consider collaborate.<br/>"+
                      "This is the only way we can move forward."
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }
        }
    }
    Rectangle {
        id: troubleshootingLayer
        width: parent.width/4.10
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.topMargin: parent.height/108
        anchors.left: recomendationsLayer.right
        anchors.leftMargin: parent.width/192
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/108
        Column {
            x: parent.width/25
            y: parent.height/43
            spacing: parent.height/216
            Text {
                text: "TROUBLESHOOTING"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "Common failures.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }
            Text {
                text: "BITTORRENT"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "<style type='text/css'> a:link{color:#757575;font-style:normal;text-decoration:none} </style>"+
                      "<font color='#757575'>The movies are not downloaded</font><br/>"+
                      "You probably have blocked the port used<br/>"+
                      "by BitTorrent in your firewall or your ISP.<br/>"+
                      "Read more &nbsp;<a href=\'http://google.com\'>Here <font color='#ffffff'>➟<font/></a><br/><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
            }
            Text {
                text: "CODECS"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36
            }
            Text {
                text: "<style type='text/css'> a:link{color:#757575;font-style:normal;text-decoration:none} </style>"+
                      "<font color='#757575'>The movies do not play</font><br/>"+
                      "You probably do not have installed<br/>"+
                      "the necessary video codecs.<br/>"+
                      "Read more &nbsp;<a href=\'http://google.com\'>Here <font color='#ffffff'>➟<font/></a><br/><br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
            }
            Text {
                text: "If you have some problem. Contact us.<br/>"
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.italic: true
                font.pixelSize: screen.height/54
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            screen.state = "cleanUp"
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: { help.forceActiveFocus() }
}
