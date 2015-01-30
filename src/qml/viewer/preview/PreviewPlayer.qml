import QtQuick 2.4
import QtMultimedia 5.4

Item {
    id: previewPlayer

    property bool downloading
    property string host
    property string url
    property string target
    property int id

    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: icon.width
        sourceSize.height: icon.height
        source: "qrc:/img-download"
        anchors.centerIn: parent
    }
    Video {
        id: player
        autoPlay: true
        fillMode: VideoOutput.Stretch
        visible: false
        enabled: false
        anchors.fill: parent
        onPlaybackStateChanged: {
            if (player.playbackState === MediaPlayer.StoppedState) {
                player.visible = false
                player.enabled = false
                icon.source = "qrc:/img-replay"
                icon.visible = true
            }
        }
    }

    Timer {
        id: delayStartDownload
        running: false
        repeat: false
        interval: 50
        onTriggered: {
            downloading = true
            cpp.preview_handler(id, host, url, target, false, false)
        }
    }
    Timer {
        id: delayStartPlayer
        running: false
        repeat: false
        interval: 50
        onTriggered: {
            icon.visible = false
            player.source = "file://"+root.tmp+target
            player.visible = true
            player.enabled = true
        }
    }
    Timer {
        id: delayDownload_err
        running: false
        repeat: false
        interval: 50
        onTriggered: { icon.source = "qrc:/img_err" }
    }

    MouseArea {
        onClicked: {
            if (!downloading && (player.playbackState === MediaPlayer.PlayingState)) {
                player.pause()
            } else if (!downloading && (player.playbackState === MediaPlayer.PausedState)) {
                player.play()
            } else if (!downloading && (player.playbackState === MediaPlayer.StoppedState)) {
                icon.visible = false
                player.visible = true
                player.enabled = true
                player.play()
            }
            view.forceActiveFocus()
        }
        anchors.fill: parent
    }

    Connections {
        target: cpp
        onSig_ret_preview: {
            if (id === previewPlayer.id) {
                if (success) {
                    downloading = false
                    delayStartPlayer.start()
                } else {
                    downloading = false
                    delayDownload_err.start()
                }
            }
        }
    }

    Component.onDestruction: {
        if ((player.playbackState === MediaPlayer.PlayingState) || (player.playbackState === MediaPlayer.PausedState)) {
            player.stop()
        } else if (downloading) {
            icon.visible = false
            downloading = false
            cpp.preview_handler(id, "", "", "", false, true)
        }
    }

    Component.onCompleted: delayStartDownload.start()
}
// Tabs hechos.
