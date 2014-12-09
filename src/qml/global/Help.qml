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
        "PLAYER",
        "CODECS",

        "TAKE PART",
        "",
        "",
        "",

        "",
        "",
        "",
        ""
    ]
    property var contents: [
        "PIG "+binary+"."+release+" DB "+database+" GPL 2.0<br/>"+
        "FREE SOFTWARE<br/>"+
        "MADE BY USERS FOR USERS",

        "",

        "ESC &nbsp; <font color='#707070'>BACK</font><br/>"+
        "CTRL H &nbsp; <font color='#707070'>HELP</font><br/>"+
        "CTRL P &nbsp; <font color='#707070'>PASSWORD</font><br/>"+
        "CTRL Q &nbsp; <font color='#707070'>CLOSE PIG</font><br/>",

        "<style type='text/css'> a:link{color:#707070;font-style:normal;text-decoration:none} </style>"+
        "THE MOVIE NOT DOWNLOAD<br/>"+
        "You probably have blocked the ports used<br/>"+
        "by BitTorrent in your firewall or ISP.<br/>"+
        "<a href=\'http://google.com\'>READ MORE <font color='#ffffff'>➟</font></a>",



        "Those who create this software agree not to<br/>"+
        "collect any user data. Note that this software<br/>"+
        "does not use encrypted connections. So it<br/>"+
        "does not ensure that third parties do not spy<br/>"+
        "what you see.",

        "",

        "SPACE &nbsp; <font color='#707070'>PLAY PAUSE</font><br/>"+
        "UP DOWN &nbsp; <font color='#707070'>VOLUME</font><br/>"+
        "LEFT RIGHT &nbsp; <font color='#707070'>MOVE ON MOVIE</font><br/>"+
        "CTRL L &nbsp; <font color='#707070'>LOOP</font>",

        "<style type='text/css'> a:link{color:#707070;font-style:normal;text-decoration:none} </style>"+
        "THE MOVIE NOT PLAY<br/>"+
        "You probably do not have installed<br/>"+
        "the necessary video codecs.<br/>"+
        "<a href=\'http://google.com\'>READ MORE <font color='#ffffff'>➟</font></a>",



        "<style type='text/css'> a:link{color:#707070;font-style:normal;text-decoration:none} </style>"+
        "CODE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP <font color='#ffffff'>➟</font></a><br/>"+
        "TORRENTS &nbsp; <a href=\'http://clarin.com\'>GITHUB.COM/PIGAPP/TORRENT <font color='#ffffff'>➟</font></a><br/>"+
        "TRANSLATIONS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/TRS <font color='#ffffff'>➟</font></a><br/>"+
        "BTC &nbsp; <font color='#707070'>1LtU9rMsQ41rCqsJAvMtw89TA5XT2dW7f9</font><br/>"+
        "BUGS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/BUGS <font color='#ffffff'>➟</font></a><br/>"+
        "SHARE &nbsp; <a href=\'http://google.com\'>PIG.IO <font color='#ffffff'>➟</font></a><br/>"+
        "CONTACT &nbsp; <font color='#707070'>PIG@MAIL.COM</font>",

        "",

        "",

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
                    anchors.leftMargin: parent.width/9
                    Text {
                        id: title
                        text: titles[index]
                        color: "white"
                        font.family: globalFont.name
                        font.pixelSize: screen.height/23
                        textFormat: Text.RichText
                        anchors.left: content.left
                        anchors.leftMargin: -screen.height/640
                    }
                    Text {
                        id: content
                        text: contents[index]
                        color: "white"
                        font.family: globalFont.name
                        font.bold: true
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
