import QtQuick 2.3

Item {
    id: news

    Row {
        spacing: parent.width/54.85
        anchors.centerIn: parent
        Column {
            spacing: screen.height/216
            Text {
                id: binaryLabel
                text: "PIG "+root.binary+"."+root.release
                color: "white"
                font.family: globalFont.name
                font.pixelSize: screen.height/23
            }
            Text {
                text: binaryNews
                color: "white"
                font.family: globalFont.name
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
                text: "DB "+root.database
                color: "white"
                font.family: globalFont.name
                font.pixelSize: screen.height/23
            }
            Text {
                text: databaseNews
                color: "white"
                font.family: globalFont.name
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
              "ESC TO CLOSE NEWS / READ MORE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/NEWS <font color='#ffffff'>➟</font></a>"
        color: "white"
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        textFormat: Text.RichText
        onLinkActivated: Qt.openUrlExternally(link)
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/154.28
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Return) {
            screen.state = "show_finder"
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: news.forceActiveFocus()
}
// Tabs hechos.
