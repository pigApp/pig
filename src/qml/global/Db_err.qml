import QtQuick 2.4

Rectangle {
    id: db_err
    color: "red"

    Column {
        spacing: -parent.height/216
        anchors.centerIn: parent
        Row {
            id: rowStatus
            spacing: screen.width/192
            Text {
                id: label
                text: "FAIL"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelInformation
                text: "DATABASE ERROR"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Row {
            spacing: screen.width/192
            anchors.horizontalCenter: rowStatus.horizontalCenter
            Text {
                id: labelGet
                text: "GET LATEST DATABASE"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/54
            }
            Text {
                id: labelLink
                text: "<style type='text/css'> a:link{color:#000000;font-style:normal;text-decoration:none} </style>"+
                      "<a href=\'http://google.com\'>DL.BINTRAY.COM/DB <font color='#ffffff'>&nbsp;&nbsp;➟</font></a>"
                textFormat: Text.RichText
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/54
                onLinkActivated: Qt.openUrlExternally(link)
                anchors.verticalCenter: labelGet.verticalCenter
                anchors.verticalCenterOffset: screen.height/1300
            }
        }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: db_err.forceActiveFocus()
}
// Tabs hechos.
