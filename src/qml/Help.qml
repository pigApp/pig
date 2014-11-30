import QtQuick 2.3

Item {
    id: help
    x: root.xA
    width: screen.width
    height: screen.height

    Grid {
        id: translucentLayerGrid
        spacing: 3
        anchors.fill: parent
        Repeater {
            id: translucentLayerRepeater
            model: 16
            Rectangle {
                id: translucentLayer
                width: parent.width/4
                height: parent.height/4
                color: Qt.rgba(0, 0, 0, 0.05)
            }
        }
    }

    Row {
        id: helpRow
        spacing: 3
        anchors.margins: parent.width/640
        anchors.topMargin: 90
        anchors.fill: parent
        Item {
            id: helpPig
            width: parent.width/4
            height: parent.height
            Column {
                spacing: parent.height/216
                anchors.top: parent.top
                anchors.topMargin: parent.height/40
                anchors.left: parent.left
                anchors.leftMargin: parent.width/12
                Text {
                    text: "PIG "+binary+"."+release+" DATABASE "+database+"<br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "COLLABORATE"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "<style type='text/css'> a:link{color:#808080;font-style:normal;text-decoration:none} </style>"+
                          "CODE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP <font color='#ff0000'>➟</font></a><br/>"+
                          "TORRENTS &nbsp; <a href=\'http://clarin.com\'>GITHUB.COM/PIGAPP/TORRENT <font color='#ff0000'>➟</font></a><br/>"+
                          "TRANSLATIONS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/TRS <font color='#ff0000'>➟</font></a><br/>"+
                          "BTC &nbsp; <font color='#808080'>1LtU9rMsQ41rCqsJAvMtw89TA5XT2dW7f9</font><br/>"+
                          "BUGS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/BUGS <font color='#ff0000'>➟</font></a><br/>"+
                          "SHARE &nbsp; <a href=\'http://google.com\'>PIG.NET <font color='#ff0000'>➟</font></a><br/>"+
                          "CONTACT &nbsp; <font color='#808080'>PIG@MAIL.COM</font><br/><br/>"
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
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "PIG Is Free Software and free.<br/>"+
                          "Made by users for users.<br/><br/>"+
                          "GPL 2.0"
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.italic: true
                    font.pixelSize: screen.height/54
                }
                Text {
                    text: "<font color='#808080'>This program is distributed in the hope that it will be useful</font><br/>"+
                          "<font color='#808080'>but WITHOUT ANY WARRANTY; without even the implied warranty</font><br/>"+
                          "<font color='#808080'>of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.</font><br/>"+
                          "<font color='#808080'>See the GNU General Public License for more details.</font>"
                    font.family: pigFont.name
                    font.bold: true
                    font.italic: true
                    font.pixelSize: screen.height/74
                }
            }
        }
        Item {
            id: helpShortcuts
            width: parent.width/4
            height: parent.height
            Column {
                spacing: parent.height/216
                anchors.top: parent.top
                anchors.topMargin: parent.height/40
                anchors.left: parent.left
                anchors.leftMargin: parent.width/12
                Text {
                    text: "KEYBOARD<br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "GENERAL"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "ESC &nbsp; <font color='#808080'>BACK</font><br/>"+
                          "CTRL H &nbsp; <font color='#808080'>HELP</font><br/>"+
                          "CTRL P &nbsp; <font color='#808080'>PASSWORD</font><br/>"+
                          "CTRL Q &nbsp; <font color='#808080'>CLOSE</font><br/>"+
                          "ALT TAB &nbsp; <font color='#808080'>PIG DESKTOP</font><br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/54
                }
                Text {
                    text: "FINDER"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "INTRO &nbsp; <font color='#808080'>FIND</font><br/>"+
                          "UP DOWN &nbsp; <font color='#808080'>OFFSET FIVE MOVIES</font><br/>"+
                          "LEFT RIGHT &nbsp; <font color='#808080'>CHANGE MOVIE</font><br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/54
                }
                Text {
                    text: "PLAYER"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "SPACE &nbsp; <font color='#808080'>PAUSE</font><br/>"+
                          "CTRL L &nbsp; <font color='#808080'>LOOP</font><br/>"+
                          "UP DOWN &nbsp; <font color='#808080'>VOLUME</font><br/>"+
                          "LEFT RIGHT &nbsp; <font color='#808080'>OFFSET</font>"
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/54
                }
            }
        }
        Item {
            id: helpRecomendations
            width: parent.width/4
            height: parent.height
            Column {
                spacing: parent.height/216
                anchors.top: parent.top
                anchors.topMargin: parent.height/40
                anchors.left: parent.left
                anchors.leftMargin: parent.width/12
                Text {
                    text: "RECOMENDATIONS<br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "RESOLUTIONS"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "◪ &nbsp; <font color='#808080'>1920X1080</font><br/>"+
                          "◪ &nbsp; <font color='#808080'>1440X975</font><br/>"+
                          "◪ &nbsp; <font color='#808080'>1280X720</font><br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/54
                }
                Text {
                    text: "UPDATES"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
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
                    text: "TRACKING"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
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
                    font.pixelSize: screen.height/23
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
        Item {
            id: helpTroubleshooting
            width: parent.width/4
            height: parent.height
            Column {
                spacing: parent.height/216
                anchors.top: parent.top
                anchors.topMargin: parent.height/40
                anchors.left: parent.left
                anchors.leftMargin: parent.width/12
                Text {
                    text: "TROUBLESHOOTING<br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "BITTORRENT"
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "<style type='text/css'> a:link{color:#808080;font-style:normal;text-decoration:none} </style>"+
                          "<font color='#808080'>The movies are not downloaded</font><br/>"+
                          "You probably have blocked the ports used<br/>"+
                          "by BitTorrent in your firewall or ISP.<br/>"+
                          "<a href=\'http://google.com\'>Read more <font color='#ff0000'>➟</font></a><br/><br/>"
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
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "<style type='text/css'> a:link{color:#808080;font-style:normal;text-decoration:none} </style>"+
                          "<font color='#808080'>The movies do not play</font><br/>"+
                          "You probably do not have installed<br/>"+
                          "the necessary video codecs.<br/>"+
                          "<a href=\'http://google.com\'>Read more <font color='#ff0000'>➟</font></a><br/><br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.italic: true
                    font.pixelSize: screen.height/54
                    textFormat: Text.RichText
                    onLinkActivated: Qt.openUrlExternally(link)
                }
                Text {
                    text: "If you have problems to use PIG, Contact us.<br/>"
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.italic: true
                    font.pixelSize: screen.height/54
                }
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            screen.state = "hide_help"
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: help.forceActiveFocus()
}
// Tabs hechos.
