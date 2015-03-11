import QtQuick 2.4

Rectangle {
    id: torrentStatus
    color: "black"

    property bool downloading: { root.status === "" }
    property string debug: root.debug
    property int timeLeft: 9

    Text {
        id: label
        text: root.status
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        visible: !downloading
        anchors.centerIn: parent
        onTextChanged: {
            if (text === "TORRENT ERROR") {
                cpp.torrent_handler("", "", 0, true)
                torrentStatus.color = "red"
                labelDebug.remove(0, labelDebug.text.length)
                labelDebug.font.pixelSize = screen.height/54
                labelDebug.color = "white"
                delayError.start()
            }
        }
    }

    Row {
        id: rowInformation
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

    Text {
        id: labelSandbox
        text: "<font color='#ffd700'>FILE NOT READY</font> RECHECK "+timeLeft
        textFormat: Text.RichText
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        visible: { movie.sandboxStatus === "FAIL" }
        anchors.top: rowInformation.bottom
        anchors.topMargin: -screen.height/72
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Timer {
        id: delaySandbox
        running: { movie.sandboxStatus === "FAIL" }
        repeat: true
        interval: 1000
        onTriggered: {
            if (timeLeft > 0) {
                labelSandbox.text = "<font color='#ffd700'>FILE NOT READY</font> RECHECK "+timeLeft
                timeLeft -= 1
            } else {
                labelSandbox.text = "<font color='#ffd700'>FILE NOT READY</font> CHECKING"
                timeLeft = 9
            }
        }
    }

    TextEdit {
        id: labelDebug
        width: parent.width
        color: Qt.rgba(0.1, 0.1, 0.1, 0.5)
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/75
        enabled: false
        readOnly: true
        cursorVisible: false
        wrapMode: Text.Wrap
        horizontalAlignment: TextEdit.AlignHCenter
        anchors.top: rowInformation.bottom
        anchors.topMargin: {
            if (movie.sandboxStatus === "FAIL")
                parent.height/54
            else if (downloading)
                -parent.height/108
        }
        anchors.horizontalCenter: parent.horizontalCenter
    }
    onDebugChanged: {
        if (labelDebug.lineCount > 5)
            labelDebug.remove(0, labelDebug.text.length)
        labelDebug.append(root.debug)
    }

    Rectangle {
       id: barDownload
       width: { (parent.width*root.mb_downloaded)/root.mb_required }
       height: parent.height/540
       color: { if (movie.sandboxStatus !== "FAIL") "white"; else "gold" }
       visible: downloading
       anchors.bottom: parent.bottom
       anchors.bottomMargin: parent.height/540
    }
    
    Timer {
        id: delayError
        interval: 5000
        onTriggered: { movie.state = "hide" }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Escape) && (x === 0) && !delayError.running) { // TODO: Asegurarse que el socket haya terminado.
            cpp.torrent_handler("", "", 0, true)
            movie.state = "hide"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    Component.onCompleted: forceActiveFocus()
}
// Tabs hechos.
