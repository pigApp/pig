import QtQuick 2.4

Rectangle {
    id: torrentStatus
    color: "black"

    property int timeLeft: 9

    Row {
        id: rowTorrentStatus
        spacing: parent.width/96
        anchors.centerIn: parent
        Row {
            spacing: screen.width/192
            Image {
                id: iconBitRate
                width: screen.width/58.18
                height: screen.height/32.72
                sourceSize.width: width
                sourceSize.height: height
                source: "qrc:/img-download"
                visible: { labelBitRate.text !== "CONNECTING" }
            }
            Text {
                id: labelBitRate
                text: { if (root.bitRate !== "") root.bitRate; else "CONNECTING" }
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
                anchors.verticalCenter: iconBitRate.verticalCenter
                anchors.verticalCenterOffset: -screen.height/432
            }
            Text {
                id: labelKb
                text: "Kb/s"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/54
                visible: { labelBitRate.text !== "CONNECTING" }
                anchors.bottom: labelBitRate.bottom
                anchors.bottomMargin: screen.height/135
            }
        }
        Row {
            spacing: screen.width/192
            Image {
                id: iconPeers
                width: screen.width/58.18
                height: screen.height/32.72
                sourceSize.width: width
                sourceSize.height: height
                source: "qrc:/img-peers"
                visible: { labelBitRate.text !== "CONNECTING" }
            }
            Text {
                id: labelPeers
                text: { if (root.peers !== 0) root.peers; else "0" }
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
                visible: { labelBitRate.text !== "CONNECTING" }
                anchors.verticalCenter: iconPeers.verticalCenter
                anchors.verticalCenterOffset: -screen.height/432
            }
        }
    }
    Text {
        id: labelSandbox
        text: "<font color='#ffd700'>FILE NOT READY</font> RECHECK "+timeLeft
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        textFormat: Text.RichText
        visible: { movie.sandboxStatus === "FAIL" }
        anchors.top: rowTorrentStatus.bottom
        anchors.topMargin: -screen.height/72
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
       id: barDownload
       width: { (parent.width*root.mb_downloaded)/root.mb_required }
       height: parent.height/540
       color: { if (movie.sandboxStatus === "FAIL") "gold"; else "white" }
       visible: { labelBitRate.text !== "CONNECTING" && labelPeers !== "PEERS 0" }
       anchors.bottom: parent.bottom
       anchors.bottomMargin: parent.height/540
    }

    Timer {
        id: delayRecheck
        running: { movie.sandboxStatus === "FAIL" }
        repeat: true
        interval: 1000
        onTriggered: {
            if (timeLeft > 0) timeLeft -= 1; else timeLeft = 9
            if (timeLeft !== 0)
                labelSandbox.text = "<font color='#ffd700'>FILE NOT READY</font> RECHECK "+timeLeft
            else
                labelSandbox.text = "<font color='#ffd700'>FILE NOT READY</font> CHECKING"
        }
    }
    
    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            movie.state = "hide"
            cpp.torrent_handler("", 0, true)
            event.accepted = true;
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: forceActiveFocus()
}
// Tabs hechos.
