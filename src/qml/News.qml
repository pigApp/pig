import QtQuick 2.3

Item {
    id: news
    anchors.fill: parent

    Row {
        id: newsRow
        spacing: parent.width/54.85
        anchors.centerIn: parent
        Column {
            spacing: screen.height/216
            Text {
                id: binaryLabel
                text: " "+" PIG "+root.binary+"."+root.release
                color: "white"
                font.family: pigFont.name
                font.pixelSize: screen.height/23
            }
            Text {
                text: binaryNews
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
        Column {
            spacing: screen.height/216
            Text {
                id: databaseLabel
                text: " "+" DB "+root.database
                color: "white"
                font.family: pigFont.name
                font.pixelSize: screen.height/23
            }
            Text {
                text: databaseNews
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/54
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }
    Text {
        id: readMoreLabel
        text: "<style type='text/css'> a:link{color:#161616;font-style:normal;text-decoration:none} </style>"+
              "READ MORE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/NEWS <font color='#007f00'>➟</font></a>"
        color: "white"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        textFormat: Text.RichText
        onLinkActivated: Qt.openUrlExternally(link)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/54
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Return) {
            screen.state = "hide_news"
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: news.forceActiveFocus()
}
// Tabs hechos.
