import QtQuick 2.4

Item {
    id: news

    Column {
        anchors.centerIn: parent
        Row {
            spacing: screen.width/128
            Text {
                id: labelBinary
                text: "PIG "+root.binary+"."+root.release
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelBinaryNews
                text: root.binaryNews
                textFormat: Text.RichText
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Row {
            spacing: screen.width/33.10
            Text {
                id: labelDatabase
                text: "DB "+root.database
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelDatabaseNews
                text: root.databaseNews
                textFormat: Text.RichText
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
    }

    Text {
        id: labelLink
        width: contentWidth
        height: contentHeight-(parent.height/360)
        text: "<style type='text/css'>a:link{color:#ffffff;text-decoration:none}</style>"+
              "<a href=\'http://google.com\'>READ MORE</a>"
        textFormat: Text.RichText
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/108
        onLinkHovered: { font.bold = !font.bold }
        onLinkActivated: Qt.openUrlExternally(link)
    }

    Keys.onPressed: {
        screen.state = "show_finder"
        event.accepted = true
    }

    Component.onCompleted: forceActiveFocus()
}
// Tabs hechos.
