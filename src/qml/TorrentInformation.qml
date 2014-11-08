import QtQuick 2.3

Rectangle {
    id: torrentInformation
    x: screen.width
    width: screen.width
    height: screen.height
    color: "black"

    property int timeLeft: 5

    Row {
        id: downloadInformationRow
        spacing: screen.width/112.94
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: progressBar.top
        anchors.bottomMargin: screen.height/216

        Text {
            id: bitRateLabel
            text: {
                if (root.bitRate !== "")
                    root.bitRate+" KB/s"
                else
                    "RECOVERING METADATA"
            }
            color: "white"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/54
        }
        Text {
            id: peersLabel
            text: {
                if (root.peers !== 0)
                    "PEERS "+root.peers
                else
                    ""
            }
            color: "white"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/54
        }
    }
    ProgressBar {
        id: progressBar
        value: root.downloaded
        visible: false
        anchors.centerIn: parent
    }
    Column {
        id: checkFileColumn
        spacing: 0
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: progressBar.bottom
        anchors.topMargin: screen.height/216
        Text {
            id: checkLabel
            text: "CHECKING FILE"
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/54
        }
        Text {
            id: recheckLabel
            text: ""
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/54
            anchors.horizontalCenter: checkLabel.horizontalCenter
        }
    }
    Timer {
        id: recheckDelay
        running: false
        repeat: true
        interval: 1000
        onTriggered: {
            if (timeLeft > 0)
                timeLeft -= 1
            else
                timeLeft = 5
            checkLabel.text = "FILE NOT READY"
            recheckLabel.text = "RECHECK ON "+timeLeft
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
                NumberAnimation { target: torrentInformation; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                PropertyAction { target: downloadInformationRow; property: "visible"; value: true }
                PropertyAction { target: progressBar; property: "visible"; value: true } 
            }
        },
        Transition {
            to: "hide"                                                               // TODO: cambiar 1920 por screen
            NumberAnimation { target: torrentInformation; easing.amplitude: 1.7; properties: "x"; to: 1920; duration: 1100; easing.type: Easing.OutQuart }
        }
    ]

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
                root.torrent_handle_qml_signal("", 0, true)
                event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quit_qml_signal()
            event.accepted = true;
        }
    }

    function reset() {
        recheckDelay.stop()
        downloadInformationRow.visible = false
        progressBar.visible = false
        checkFileColumn.visible = false
        root.required = 0
        root.downloaded = 0
        root.bitRate = ""
        root.peers = 0
    }

    Connections {
        target: cppSignals
        onChecking_file_signal: {
            checkFileColumn.visible = true
            recheckDelay.start()
        }
        onFile_ready_signal: reset()
        onHide_torrent_information_signal: {
            reset()
            torrentInformation.state = "hide"
        }
    }

    Component.onCompleted: {
        torrentInformation.state = "show"
        torrentInformation.forceActiveFocus()
    }
}
