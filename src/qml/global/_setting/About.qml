import QtQuick 2.4

Item {
    id: about

    Column {
        spacing: -screen.height/216
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: labelData
            text: "PIG "+root.binary+"."+root.release+" · DB "+root.database+" · GPL 2.0"
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: labelMovies
            text: "<style type='text/css'>a:link{color:#ffffff;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>MOVIES</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            id: labelCode
            text: "<style type='text/css'>a:link{color:#ffffff;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>CODE</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            id: labelBugs
            text: "<style type='text/css'>a:link{color:#ffffff;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>BUGS</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            id: labelContact
            text: "PIG@MAIL.COM"
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
    }
}
// Tabs hechos.
