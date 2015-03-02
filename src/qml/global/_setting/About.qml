import QtQuick 2.4

Item {
    id: about

    Text {
        id: label
        text: "PIG "+root.binary+"."+root.release+" · DB "+root.database+" · GPL 2.0"+"<br/>"+
              "<style type='text/css'> a:link{color:#202020;font-style:normal;text-decoration:none} </style>"+
              "MOVIES &nbsp; <a href=\'http://clarin.com\'>GITHUB.COM/PIGAPP/MOVIES <font color='#ffffff'>&nbsp;&nbsp;➟</font></a><br/>"+
              "CODE &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP <font color='#ffffff'>&nbsp;&nbsp;➟</font></a><br/>"+
              "BUGS &nbsp; <a href=\'http://google.com\'>GITHUB.COM/PIGAPP/BUGS <font color='#ffffff'>&nbsp;&nbsp;➟</font></a><br/>"+
              "BTC &nbsp; <font color='#202020'>1LtU9rMsQ41rCqsJAvMtw89TA5XT2dW7f9</font><br/>"+
              "CONTACT &nbsp; <font color='#202020'>PIG@MAIL.COM</font><br/>"
        textFormat: Text.RichText
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        anchors.left: parent.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: parent.verticalCenter
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
// Tabs hechos.
