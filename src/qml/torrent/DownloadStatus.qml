import QtQuick 2.3

Rectangle {
    id: downloadStatus
    color: "black"
    anchors.fill: parent

    property int timeLeft: 10

    Row {
        id: downloadStatusRow
        spacing: parent.width/96
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: bitRateLabel
            text: {
                if (root.bitRate !== "")
                    root.bitRate+" KB/s"
                else
                    "RECOVERING METADATA"
            }
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: peersLabel
            text: {
                if (root.peers !== 0)
                    "PEERS "+root.peers
                else
                    "PEERS 0"
            }
            color: "white"
            visible: { bitRateLabel.text !== "RECOVERING METADATA" }
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
        visible: { if (handler.sandboxStatus === "fail") true; else false }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: downloadStatusRow.bottom
        anchors.topMargin: screen.height/108
    }
    Timer {
        id: recheckDelay
        running: { if (handler.sandboxStatus === "fail") true; else false }
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

    Rectangle {
       id: bar
       width: { (parent.width*root.downloaded)/root.required }
       height: 2
       color: { if (handler.sandboxStatus === "fail") "yellow"; else "white" }
       visible: { bitRateLabel.text !== "RECOVERING METADATA" && peersLabel !== "PEERS 0" }
       anchors.bottom: parent.bottom
       anchors.bottomMargin: 2
    }
    
    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            root.signal_qml_torrent_handler("", 0, true)
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: downloadStatus.forceActiveFocus()
}
