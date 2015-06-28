import QtQuick 2.4

Item {
    id: about

    Column {
        id: columnAbout
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
            width: contentWidth
            height: contentHeight-(screen.height/108)
            text: "<style type='text/css'>a:link{color:#FFFFFF;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>MOVIES</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            id: labelCode
            width: contentWidth
            height: contentHeight-(screen.height/108)
            text: "<style type='text/css'>a:link{color:#FFFFFF;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>CODE</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            id: labelBugs
            width: contentWidth
            height: contentHeight-(screen.height/108)
            text: "<style type='text/css'>a:link{color:#FFFFFF;text-decoration:none}</style>"+
                  "<a href=\'http://google.com\'>BUGS</a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            onLinkHovered: { font.bold = !font.bold }
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            id: labelDonate
            width: contentWidth
            height: contentHeight-(screen.height/108)
            text: "DONATE"
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onEntered: { labelDonate.font.bold = false }
                onHoveredChanged: { labelDonate.font.bold = true }
                onClicked: {
                    if (!labelWallet.visible) {
                        labelDonate.text = "BTC"
                        labelWallet.visible = true
                    } else {
                        labelDonate.text = "DONATE"
                        labelWallet.visible = false
                    }
                }
            }
        }
    }
    TextEdit {
        id: labelWallet
        text: "159yedWATJUr9r4NGe2A8Zw87rCzbGwNFV"
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/54
        visible: false
        selectByMouse: true
        selectionColor: "black"
        readOnly: true
        anchors.left: columnAbout.left
        anchors.leftMargin: screen.width/21.33
        anchors.bottom: columnAbout.bottom
        anchors.bottomMargin: -screen.height/540
    }
}
// Tabs hechos.
