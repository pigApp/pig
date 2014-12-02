import QtQuick 2.3

Item {
    id: help
    x: root.xA
    width: screen.width
    height: screen.height

    property var titles: [
        "LICENSE",
        "SETUP",
        "GLOBAL",
        "BITTORRENT",

        "TRACKING",
        "",
        "FINDER",
        "CODECS",

        "TAKE PART",
        "",
        "PLAYER",
        "",

        "",
        "",
        "",
        ""
    ]
    property var contents: [
        "PIG "+binary+"."+release+" DB "+database+" GPL 2.0. Free Software and free.<br/>"+
        "Made by users for users.<br/><br/>"+
        "<font color='#707070'>This program is distributed in the hope that it will</font><br/>"+
        "<font color='#707070'>but WITHOUT ANY WARRANTY; without even</font><br/>"+
        "<font color='#707070'>of MERCHANTABILITY or FITNESS FOR.</font><br/>"+
        "<font color='#707070'>See the GNU General Public License.</font>",

        "",

        "<font color='#000000'>KEYS</font><br/>"+
        "CTRL H &nbsp; <font color='#707070'>HELP</font><br/>"+
        "CTRL P &nbsp; <font color='#707070'>PASSWORD</font><br/>"+
        "CTRL Q &nbsp; <font color='#707070'>CLOSE PIG</font><br/>"+
        "ESC &nbsp; <font color='#707070'>BACK</font><br/>"+
        "ALT TAB &nbsp; <font color='#707070'>MOVE ON PIG DESKTOP</font>",

        "<style type='text/css'> a:link{color:#707070;font-style:normal;text-decoration:none} </style>"+
        "<font color='#000000'>TROUBLE</font><br/>"+
        "<font color='#707070'>The movies do not download</font><br/>"+
        "You probably have blocked the ports used<br/>"+
        "by BitTorrent in your firewall or ISP.<br/>"+
        "<a href=\'http://google.com\'>READ MORE <font color='#ffffff'>➟</font></a>",



        "Those who create this software agree not to<br/>"+
        "collect any user data. Note that this software<br/>"+
        "does not use encrypted connections. So it<br/>"+
        "does not ensure that third parties do not spy<br/>"+
        "what you see.",

        "",

        "<font color='#000000'>KEYS</font><br/>"+
        "INTRO &nbsp; <font color='#707070'>FIND MOVIES</font><br/>"+
        "ARROWS &nbsp; <font color='#707070'>MOVE ON MOVIES</font><br/><br/>",

        "<style type='text/css'> a:link{color:#707070;font-style:normal;text-decoration:none} </style>"+
        "<font color='#000000'>TROUBLE</font><br/>"+
        "<font color='#707070'>The movies do not play</font><br/>"+
        "You probably do not have installed<br/>"+
        "the necessary video codecs.<br/>"+
        "<a href=\'http://google.com\'>READ MORE <font color='#ffffff'>➟</font></a>",



        "<style type='text/css'> a:link{color:#707070;font-style:normal;text-decoration:none} </style>"+
        "CODE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP <font color='#ffffff'>➟</font></a><br/>"+
        "TORRENTS &nbsp; <a href=\'http://clarin.com\'>GITHUB.COM/PIGAPP/TORRENT <font color='#ffffff'>➟</font></a><br/>"+
        "TRANSLATIONS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/TRS <font color='#ffffff'>➟</font></a><br/>"+
        "BTC &nbsp; <font color='#707070'>1LtU9rMsQ41rCqsJAvMtw89TA5XT2dW7f9</font><br/>"+
        "BUGS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/BUGS <font color='#ffffff'>➟</font></a><br/>"+
        "SHARE &nbsp; <a href=\'http://google.com\'>PIG.NET <font color='#ffffff'>➟</font></a><br/>"+
        "CONTACT &nbsp; <font color='#707070'>PIG@MAIL.COM</font>",

        "",

        "<font color='#000000'>KEYS</font><br/>"+
        "SPACE &nbsp; <font color='#707070'>PLAY PAUSE</font><br/>"+
        "ARROWS UP DOWN &nbsp; <font color='#707070'>VOLUME</font><br/>"+
        "ARROWS LEFT RIGHT &nbsp; <font color='#707070'>MOVE ON MOVIE</font><br/>"+
        "CTRL L &nbsp; <font color='#707070'>LOOP</font>",

        "",



        /*
        "If you use this software, consider collaborate.<br/>"+
        "This is the only way we can move forward.",
        */
        "",

        "",

        "",

        ""
        /*
        "If you have problems to use PIG<br/>"+
        "feel free to contact us."
        */
    ]

    Grid {
        id: grid
        spacing: 3
        anchors.fill: parent
        Repeater {
            model: 16
            delegate: Rectangle {
                id: box
                width: parent.width/4
                height: parent.height/4
                color: Qt.rgba(0, 0, 0, 0.05)
                Column {
                    spacing: parent.height/216
                    anchors.top: parent.top
                    anchors.topMargin: parent.height/40
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width/20
                    Text {
                        text: titles[index]
                        color: "white"
                        font.family: pigFont.name
                        font.pixelSize: screen.height/23
                        textFormat: Text.RichText
                    }
                    Text {
                        text: contents[index]
                        color: "white"
                        font.family: pigFont.name
                        font.bold: true
                        font.italic: true
                        font.pixelSize: screen.height/54
                        textFormat: Text.RichText
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
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
