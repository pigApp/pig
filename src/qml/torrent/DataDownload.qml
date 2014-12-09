import QtQuick 2.3

Rectangle {
    id: downloadInformation
    x: screen.width
    width: screen.width
    height: screen.height
    color: "black"

    property int timeLeft: 5

    Row {
        id: downloadInformationRow
        spacing: screen.width/96
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        //anchors.bottom: progressBar.top
        //anchors.bottomMargin: screen.height/216
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
            //font.bold: true
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
            //font.bold: true
            font.pixelSize: screen.height/23
        }
        onVisibleChanged: {
            if (visible)
                downloadInformation.forceActiveFocus()
        }
    }

    Row {
        id: checkFileRow
        spacing: screen.width/96
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: downloadInformationRow.bottom
        anchors.topMargin: screen.height/108
        Text {
            id: checkLabel
            text: "CHECKING FILE"
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: recheckLabel
            text: ""
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: globalFont.name
            font.pixelSize: screen.height/23
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

    ProgressBar {
        id: downloadProgress
        value: root.downloaded
        visible: { bitRateLabel.text !== "RECOVERING METADATA" && peersLabel !== "PEERS 0" && !checkFileRow.visible }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
    }

    states: [
        State {
            name: "show"
        },
        State {
            name: "hide"
        },
        State {
            name: "show_sandbox"
            PropertyChanges { target: checkFileRow; visible: true; restoreEntryValues: false }
            PropertyChanges { target: recheckDelay; running: true; restoreEntryValues: false }
        }
    ]
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {                                                                 //root.xA
                NumberAnimation { target: downloadInformation; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                PropertyAction { target: downloadInformationRow; property: "visible"; value: true }
            }
        },
        Transition {
            to: "hide"
            NumberAnimation { target: downloadInformation; easing.amplitude: 1.7; properties: "x"; to: root.screenWidth; duration: 1100; easing.type: Easing.OutQuart }
        }
    ]

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
                root.signal_qml_torrent_handler("", 0, true)
                event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    function reset() { // REVISAR
        recheckDelay.stop()
        downloadInformationRow.visible = false
        downloadProgress.visible = false
        checkFileRow.visible = false
        root.required = 0
        //root.downloaded = 0
        //root.bitRate = ""
        //root.peers = 0
    }

    Connections {
        target: cppSignals
        /*
        onSignal_checking_file: {
            checkFileRow.visible = true
            recheckDelay.start()
        }
        onSignal_file_ready: reset()
        */
        onSignal_hide_torrent_handler: {
            reset()
            downloadInformation.state = "hide"
        }
    }

    Component.onCompleted: { downloadInformation.state = "show" }
}
