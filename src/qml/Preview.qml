import QtQuick 2.3
import QtMultimedia 5.0

Item {
    id: preview
    anchors.fill: parent

    property bool onConnect
    property bool stopPlayer: root.stopPreview
    property string host
    property string url
    property string path
    property string target
    property int id

    Rectangle {
        id: translucentLayer
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.fill: parent
        MouseArea {
            onClicked: {
                if (!onConnect) {
                    onConnect = true
                    previewLabel.visible = false
                    icon.visible = true
                    root.signal_qml_preview_handler(host, url, "", target, id, false, false, false)
                }
            }
            anchors.fill: parent
        }
    }
    Text {
        id: previewLabel
        text: "PREVIEW"
        color: "white"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/24
        opacity: 0.4
        anchors.centerIn: parent
        MouseArea {
            onClicked: {
                if (!onConnect) {
                    onConnect = true
                    previewLabel.visible = false
                    icon.visible = true
                    root.signal_qml_preview_handler(host, url, "", target, id, false, false, false)
                }
            }
            anchors.fill: parent
        }
    }

    Video {
        id: player
        visible: false
        enabled: false
        fillMode: VideoOutput.Stretch
        anchors.fill: parent
        onPlaybackStateChanged: {
            if (player.playbackState == MediaPlayer.StoppedState) {
                player.opacity = 0
                icon.visible = true
                icon.enabled = true
            } else {
                icon.visible = false
                icon.enabled = false
                icon.source = "/resources/images/output/preview/replay.png"
                player.opacity = 1
            }
        }
        MouseArea {
            onClicked: {
                if (player.playbackState == MediaPlayer.PlayingState)
                    player.pause()
                else
                    player.play()
            }
            anchors.fill: parent
        }
    }
    Timer {
        id: playerDelay
        running: false
        repeat: false
        interval: 10
        onTriggered: {
            player.source = "file://"+path+target
            player.visible = true
            player.enabled = true
            player.play()
        }
    }

    Image {
        id: icon
        width: screen.width/22.58
        height: screen.height/12.70
        sourceSize.width: screen.width/22.58
        sourceSize.height: screen.height/12.70
        source: "/resources/images/output/preview/network/icon.png"
        opacity: 0.4
        visible: false
        enabled: false
        anchors.centerIn: parent
        MouseArea {
            onClicked: player.play()
            anchors.fill: parent
        }
    }

    Timer {
        id: errorDelay
        running: false
        repeat: false
        interval: 10
        onTriggered: {
            icon.visible = false
            icon.enabled = false
            previewLabel.text = "PREVIEW ERROR"
            previewLabel.visible = true
        }
    }

    onStopPlayerChanged: {
        if (stopPlayer && (player.playbackState == MediaPlayer.PlayingState || player.playbackState == MediaPlayer.PausedState)) {
            player.stop()
        } else if (stopPlayer && onConnect) {
            icon.visible = false
            previewLabel.visible = true
            onConnect = false
            root.signal_qml_preview_handler("", "", "", "", id, false, false, true)
        }
    }

    Connections {
        target: cppSignals
        onSignal_success_preview: {
            if (id === preview.id) {
                onConnect = false
                preview.path = path
                playerDelay.start()
            }
        }
        onSignal_fail_preview: {
            if (id === preview.id) {
                onConnect = false
                errorDelay.start()
            }
        }
    }
}
// Tabs hechos.
