import QtQuick 2.4

Rectangle {
    id: help
    color: "#00490c"

    property var titles: [
        "LICENSE",
        "KEYS GLOBAL",
        "BITTORRENT",

        "TAKE PART",
        "KEYS MOVIES",
        "CODECS",

        "TRACKING",
        "KEYS PLAYER",
        "FOCUS",

        "CONTACT",
        "",
        ""
    ]
    property var contents: [
        "PIG "+binary+"."+release+" DB "+database+"<br/>"+
        "GPL 2.0 Free Software.<br/>"+
        "MADE BY USERS FOR USERS",

        "ESC &nbsp; <font color='#161616'>BACK</font><br/>"+
        "CTRL H &nbsp; <font color='#161616'>HELP</font><br/>"+
        "CTRL P &nbsp; <font color='#161616'>PASSWORD</font><br/>"+
        "CTRL Q &nbsp; <font color='#161616'>CLOSE PIG</font>",

        "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
        "THE MOVIE NOT DOWNLOAD<br/>"+
        "Probably are blocked the ports used<br/>"+
        "by BitTorrent in the firewall or ISP.<br/>"+
        "<a href=\'http://google.com\'>READ MORE <font color='#ffffff'>&nbsp;&nbsp;➟</font></a>",


        "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
        "MOVIES &nbsp; <a href=\'http://clarin.com\'>GITHUB.COM/PIGAPP/MOVIES <font color='#ffffff'>&nbsp;&nbsp;➟</font></a><br/>"+
        "CODE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP <font color='#ffffff'>&nbsp;&nbsp;➟</font></a><br/>"+
        "BUGS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/BUGS <font color='#ffffff'>&nbsp;&nbsp;➟</font></a><br/>"+
        "BTC &nbsp; <font color='#161616'>1LtU9rMsQ41rCqsJAvMtw89TA5XT2dW7f9</font><br/>",

        "LEFT RIGHT &nbsp; <font color='#161616'>MOVE ON MOVIES</font><br/>"+
        "UP DOWN &nbsp; <font color='#161616'>MORE LESS FIVE MOVIES</font>",

        "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
        "THE MOVIE NOT PLAY<br/>"+
        "Probably do not are installed<br/>"+
        "the necessary video codecs.<br/>"+
        "<a href=\'http://google.com\'>READ MORE <font color='#ffffff'>&nbsp;&nbsp;➟</font></a>",


        "Those who create this software agree not to<br/>"+
        "collect any user data.",

        "SPACE &nbsp; <font color='#161616'>PLAY PAUSE</font><br/>"+
        "LEFT RIGHT &nbsp; <font color='#161616'>OFFSET</font><br/>"+
        "UP DOWN &nbsp; <font color='#161616'>VOLUME</font><br/>"+
        "CTRL L &nbsp; <font color='#161616'>LOOP</font>",

        "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
        "THE KEYBOARD NOT WORK<br/>"+
        "Click on the screen to regain focus.",


        "<font color='#161616'>PIG@MAIL.COM</font>",

        "",

        ""
    ]

    Grid {
        id: grid
        columns: 3
        anchors.top: parent.top
        anchors.topMargin: parent.height/8.64
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: parent.width/12.8
        Repeater {
            model: 12
            delegate:
            Item {
                width: screen.width/3
                height: screen.height/5
                Column {
                    spacing: screen.height/216
                    anchors.top: parent.top
                    anchors.left: parent.left
                    Text {
                        id: labelTitle
                        text: titles[index]
                        textFormat: Text.RichText
                        color: "white"
                        font.family: fontGlobal.name
                        font.pixelSize: screen.height/23
                        anchors.left: labelContent.left
                        anchors.leftMargin: -screen.height/640
                    }
                    Text {
                        id: labelContent
                        text: contents[index]
                        textFormat: Text.RichText
                        color: "white"
                        font.family: fontGlobal.name
                        font.bold: true
                        font.pixelSize: screen.height/54
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
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: help.forceActiveFocus()
}
// Tabs hechos.
