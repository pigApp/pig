import QtQuick 2.3
import QtMultimedia 5.0

Item {
    id: preview
    anchors.fill: parent

    property bool downloading
    property string host
    property string url
    property string target
    property string path
    property int id

    Image {
        id: icon
        width: screen.width/60
        height: screen.height/33.75
        sourceSize.width: screen.width/60
        sourceSize.height: screen.height/33.75
        source: "qrc:/img-download"
        opacity: 0.4
        anchors.centerIn: parent
    }

    Video {
        id: player
        autoPlay: true
        visible: false
        enabled: false
        fillMode: VideoOutput.Stretch
        anchors.fill: parent
        onPlaybackStateChanged: {
            if (player.playbackState === MediaPlayer.StoppedState) {
                player.visible = false
                player.enabled = false
                icon.source = "qrc:/img-play"
                icon.visible = true
            }
        }
    }
    MouseArea {
        id: controlsPlayerMouseArea
        onClicked: {
            if (!downloading && player.playbackState === MediaPlayer.PlayingState) {
                player.pause()
                icon.source = "qrc:/img-play"
                icon.visible = true
            } else if (!downloading && player.playbackState === MediaPlayer.PausedState) {
                icon.visible = false
                player.play()
            } else if (!downloading && player.playbackState === MediaPlayer.StoppedState) {
                icon.visible = false
                player.visible = true
                player.enabled = true
                player.play()
            }
            view.forceActiveFocus()
        }
        anchors.fill: parent
    }

    Timer {
        id: startDownloadDelay
        running: false
        repeat: false
        interval: 50
        onTriggered: {
            downloading = true
            root.signal_qml_preview_handler(host, url, "", target, id, false, false)
        }
    }
    Timer {
        id: errorDownloadDelay
        running: false
        repeat: false
        interval: 50
        onTriggered: { icon.source = "qrc:/img-download_err" }
    }
    Timer {
        id: startPlayerDelay
        running: false
        repeat: false
        interval: 50
        onTriggered: {
            icon.visible = false
            player.source = "file://"+path+target
            player.visible = true
            player.enabled = true
        }
    }

    Connections {
        target: cppSignals
        onSignal_ret__preview: {
            if (id === preview.id) {
                if (success) {
                    downloading = false
                    preview.path = path
                    startPlayerDelay.start()
                } else {
                    downloading = false
                    errorDownloadDelay.start()
                }
            }
        }
    }

    Component.onDestruction: {
        if (player.playbackState === MediaPlayer.PlayingState || player.playbackState === MediaPlayer.PausedState) {
            player.stop()
        } else if (downloading) {
            icon.visible = false
            downloading = false
            root.signal_qml_preview_handler("", "", "", "", id, false, true)
        }
    }

    Component.onCompleted: startDownloadDelay.start()
}
// Tabs hechos.
