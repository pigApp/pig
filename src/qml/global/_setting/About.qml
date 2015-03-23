import QtQuick 2.4

Rectangle {
    id: about
    color: "black"

    Text {
        id: label
        text: "PIG "+root.binary+"."+root.release+" · DB "+root.database+" · GPL 2.0"+"<br/>"+
              "<style type='text/css'>a:link{color:#ffffff;font-style:normal;text-decoration:none}</style>"+
              "<a href=\'http://google.com\'>MOVIES &nbsp;&nbsp;➟</a><br/>"+
              "<a href=\'http://google.com\'>CODE &nbsp;&nbsp;➟</a><br/>"+
              "<a href=\'http://google.com\'>BUGS &nbsp;&nbsp;➟</a><br/>"+
              "BTC &nbsp; <font color='#808080'>1LtU9rMsQ41rCqsJAvMtw89TA5XT2dW7f9</font><br/>"+
              "CONTACT &nbsp; <font color='#808080'>PIG@MAIL.COM</font>"
        textFormat: Text.RichText
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
// Tabs hechos.
