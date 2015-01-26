import QtQuick 2.4

Item {
    id: news

    Row {
        spacing: parent.width/96
        anchors.centerIn: parent
        Column {
            Text {
                id: binaryLabel
                text: "PIG "+root.binary+"."+root.release
                color: "white"
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                text: binaryNews
                textFormat: Text.RichText
                color: "white"
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/54
            }
        }
        Column {
            Text {
                id: databaseLabel
                text: "DB "+root.database
                color: "white"
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                text: databaseNews
                textFormat: Text.RichText
                color: "white"
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/54
            }
        }
    }
    Row {
        spacing: parent.width/192
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/154.28
        Text {
            id: readMoreLabel
            text: "READ MORE"
            color: "white"
            font.family: globalFont.name
            font.bold: true
            font.pixelSize: screen.height/54
        }
        Text {
            id: linkLabel
            text: "<style type='text/css'> a:link{color:#000000;font-style:normal;text-decoration:none} </style>"+
                  "<a href=\'http://google.com\'>GITHUB.COM/PIGAPP/NEWS <font color='#ffffff'>&nbsp;&nbsp;➟</font></a>"
            textFormat: Text.RichText
            font.family: globalFont.name
            font.bold: true
            font.pixelSize: screen.height/54
            onLinkActivated: Qt.openUrlExternally(link)
            anchors.verticalCenter: readMoreLabel.verticalCenter
        }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        } else {
            screen.state = "show_finder"
            event.accepted = true;
        }
    }

    Component.onCompleted: news.forceActiveFocus()
}
// Tabs hechos.
