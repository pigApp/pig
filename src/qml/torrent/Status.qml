import QtQuick 2.4

Item {
    id: status

    property int timeLeft: 9

    Row {
        id: statusRow
        spacing: parent.width/96
        anchors.centerIn: parent
        Row {
            id: bitRateRow
            spacing: screen.width/192
            Image {
                id: bitRateIcon
                width: screen.width/58.18
                height: screen.height/32.72
                sourceSize.width: bitRateIcon.width
                sourceSize.height: bitRateIcon.height
                source: "qrc:/img-download"
                visible: { bitRateLabel.text !== "CONNECTING" }
            }
            Text {
                id: bitRateLabel
                text: { if (root.bitRate !== "") root.bitRate; else "CONNECTING" }
                color: "white"
                font.family: globalFont.name
                font.pixelSize: screen.height/23
                anchors.verticalCenter: bitRateIcon.verticalCenter
                anchors.verticalCenterOffset: -screen.height/432
            }
            Text {
                id: kbLabel
                text: "Kb/s"
                color: "white"
                font.family: globalFont.name
                font.bold: true
                font.pixelSize: screen.height/54
                visible: { bitRateLabel.text !== "CONNECTING" }
                anchors.bottom: bitRateLabel.bottom
                anchors.bottomMargin: screen.height/135
            }
        }
        Row {
            id: peersRow
            spacing: screen.width/192
            Image {
                id: peersIcon
                width: screen.width/58.18
                height: screen.height/32.72
                sourceSize.width: peersIcon.width
                sourceSize.height: peersIcon.height
                source: "qrc:/img-peers"
                visible: { bitRateLabel.text !== "CONNECTING" }
            }
            Text {
                id: peersLabel
                text: { if (root.peers !== 0) root.peers; else "0" }
                color: "white"
                font.family: globalFont.name
                font.pixelSize: screen.height/23
                visible: { bitRateLabel.text !== "CONNECTING" }
                anchors.verticalCenter: peersIcon.verticalCenter
                anchors.verticalCenterOffset: -screen.height/432
            }
        }
    }
    Text {
        id: sandboxLabel
        text: "<font color='#ffff00'>FILE NOT READY</font> RECHECK "+timeLeft
        color: "white"
        font.family: globalFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        textFormat: Text.RichText
        visible: { torrentHandler.sandboxStatus === "fail" }
        anchors.top: statusRow.bottom
        anchors.topMargin: -screen.height/72
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
       id: bar
       width: { (parent.width*root.downloaded)/root.required }
       height: parent.height/540
       color: { if (torrentHandler.sandboxStatus === "fail") "yellow"; else "white" }
       visible: { bitRateLabel.text !== "CONNECTING" && peersLabel !== "PEERS 0" }
       anchors.bottom: parent.bottom
       anchors.bottomMargin: parent.height/540
    }

    Timer {
        id: recheckDelay
        running: { torrentHandler.sandboxStatus === "fail" }
        repeat: true
        interval: 1000
        onTriggered: {
            if (timeLeft > 0)
                timeLeft -= 1
            else
                timeLeft = 9

            if (timeLeft !== 0)
                sandboxLabel.text = "<font color='#ffff00'>FILE NOT READY</font> RECHECK "+timeLeft
            else
                sandboxLabel.text = "<font color='#ffff00'>FILE NOT READY</font> RECHECK <font color='#ffff00'>checking</font>"
        }
    }
    
    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            torrentHandler.state = "hide"
            root.signal_qml_torrent_handler("", 0, true)
            event.accepted = true;
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: status.forceActiveFocus()
}
// Tabs hechos.
