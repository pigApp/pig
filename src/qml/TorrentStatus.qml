import QtQuick 2.4

Rectangle {
    id: torrentStatus
    x: root.xa
    width: screen.width
    height: screen.height
    color: "transparent"
    enabled: { x === 0 }
    visible: { x === 0 }

    property bool downloading: { root.status === "" }

    Row {
        id: rowStatus
        spacing: parent.width/192
        visible: !downloading
        anchors.centerIn: parent
        Image {
            id: icon
            width: screen.width/58.18
            height: screen.height/32.72
            sourceSize.width: width
            sourceSize.height: height
            source: "qrc:/img-download"
        }
        Text {
            id: label
            text: root.status
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
            anchors.verticalCenter: icon.verticalCenter
            anchors.verticalCenterOffset: -screen.height/432
            onTextChanged: {
                if (text === "TORRENT ERROR") {
                    cpp.torrent("", "", 0, true)
                    torrentStatus.color = "red"
                    labelDebug.remove(0, labelDebug.text.length)
                    labelDebug.font.pixelSize = screen.height/54
                    labelDebug.color = "white"
                    delayError.start()
                }
            }
        }
    }

    Row {
        id: rowStats
        spacing: parent.width/96
        visible: downloading
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
            }
            Text {
                id: labelBitRate
                text: root.bitRate
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
            }
            Text {
                id: labelPeers
                text: { if (root.peers !== 0) root.peers; else "0" }
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
                anchors.verticalCenter: iconPeers.verticalCenter
                anchors.verticalCenterOffset: -screen.height/432
            }
        }
    }

    Timer {
        id: delayError
        interval: 5000
        onTriggered: { torrentStatus.state = "hide" }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Escape)
            && (x === 0) && !delayError.running) {
            cpp.torrent("", "", "", 0, true)
            torrentStatus.state = "hide"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q)
            && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    states: [
        State {
            name: "show"
        },
        State {
            name: "hide"
        }
    ]
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"
                        ; to: -(screen.width+10); duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"
                        ; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"
                        ; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "blurRadius"
                        ; to: 0; duration: 1100; easing.type: Easing.OutQuart }

                }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"
                        ; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"
                        ; to: screen.width+10; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"
                        ; to: 0.8; duration: 1200; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "blurRadius"
                        ; to: 64; duration: 1200; easing.type: Easing.OutQuart }
                 }
                PropertyAction { target: screen; property: "state"; value: "hide_torrent_status" }
            }
        }
    ]

    Component.onCompleted: {
        torrentStatus.state = "show"
        forceActiveFocus()
    }
}
// Tabs hechos.
