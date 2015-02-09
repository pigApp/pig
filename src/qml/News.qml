import QtQuick 2.4

Item {
    id: news
    visible: false

    Row {
        spacing: parent.width/96
        anchors.centerIn: parent
        Column {
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
                font.bold: true
                font.pixelSize: screen.height/54
            }
        }
        Column {
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
            id: labelReadMore
            text: "READ MORE"
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/54
        }
        Text {
            id: labelLink
            text: "<style type='text/css'> a:link{color:#000000;font-style:normal;text-decoration:none} </style>"+
                  "<a href=\'http://google.com\'>GITHUB.COM/PIGAPP/NEWS <font color='#ffffff'>&nbsp;&nbsp;➟</font></a>"
            textFormat: Text.RichText
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/54
            onLinkActivated: Qt.openUrlExternally(link)
            anchors.verticalCenter: labelReadMore.verticalCenter
        }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true;
        } else {
            screen.state = "show_finder"
            event.accepted = true;
        }
    }

    states: State {
        name: "show"
    }
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenR"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenG"; to: 0.28; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenB"; to: 0.047; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.9; duration: 600; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: news; property: "visible"; value: true }
            }
        }
    ]

    Component.onCompleted: {
        news.state = "show"
        news.forceActiveFocus()
    }
}
// Tabs hechos.
