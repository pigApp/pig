import QtQuick 2.4

Item {
    id: news

    Column {
        anchors.left: parent.left
        anchors.leftMargin: parent.width/4.8
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: labelBinary
            text: "PIG "+root.binary+"."+root.release
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            anchors.left: labelBinaryNews.left
            anchors.leftMargin: screen.width/54.85
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
    Column {
        anchors.right: parent.right
        anchors.rightMargin: parent.width/4.8
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: labelDatabase
            text: "DB "+root.database
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            anchors.left: labelDatabaseNews.left
            anchors.leftMargin: screen.width/54.85
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

    Text {
        id: labelLink
        text: "<style type='text/css'>a:link{color:#ffffff;text-decoration:none}</style>"+
              "<a href=\'http://google.com\'>READ MORE</a>"
        textFormat: Text.RichText
        font.family: fontGlobal.name
        font.pixelSize: screen.height/54
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/154.28
        onLinkHovered: { font.bold = !font.bold }
        onLinkActivated: Qt.openUrlExternally(link)
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        } else {
            screen.state = "show_finder"
            event.accepted = true
        }
    }

    Component.onCompleted: forceActiveFocus()
}
// Tabs hechos.
