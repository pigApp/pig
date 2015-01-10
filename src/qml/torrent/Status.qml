import QtQuick 2.3

Item {
    id: status

    property int timeLeft: 10

    Row {
        id: statusRow
        spacing: parent.width/96
        anchors.centerIn: parent
        Text {
            id: bitRateLabel
            text: { if (root.bitRate !== "") root.bitRate+" KB/s"; else "CONNECTING" }
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: peersLabel
            text: { if (root.peers !== 0) "PEERS "+root.peers; else "PEERS 0" }
            color: "white"
            visible: { bitRateLabel.text !== "CONNECTING" }
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
    }
    Text {
        id: sandboxLabel
        text: "FILE NOT READY - RECHECK "+timeLeft
        color: Qt.rgba(0.1, 0.1, 0.1, 1)
        font.family: globalFont.name
        font.pixelSize: screen.height/56
        visible: { if (torrentHandler.sandboxStatus === "fail") true; else false }
        anchors.top: statusRow.bottom
        anchors.topMargin: screen.height/108
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Rectangle {
       id: bar
       width: { (parent.width*root.downloaded)/root.required }
       height: 2
       color: { if (torrentHandler.sandboxStatus === "fail") "yellow"; else "white" }
       visible: { bitRateLabel.text !== "CONNECTING" && peersLabel !== "PEERS 0" }
       anchors.bottom: parent.bottom
       anchors.bottomMargin: 2
    }

    Timer {
        id: recheckDelay
        running: { if (torrentHandler.sandboxStatus === "fail") true; else false }
        repeat: true
        interval: 1000
        onTriggered: {
            if (timeLeft > 0)
                timeLeft -= 1
            else
                timeLeft = 10
            sandboxLabel.text = "FILE NOT READY - RECHECK "+timeLeft
        }
    }
    
    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            torrentHandler.state = "hide"
            root.signal_qml_torrent_handler("", 0, true)
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: status.forceActiveFocus()
}
// Tabs hechos.
