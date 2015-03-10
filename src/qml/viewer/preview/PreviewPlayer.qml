import QtQuick 2.4
import QtMultimedia 5.4

Item {
    id: previewPlayer

    property bool quit: root.preview_quit
    property bool cached
    property bool downloading
    property string host
    property string url
    property string target: id_cache+"p.mp4"
    property int id_private
    property int id_cache

    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: width
        sourceSize.height: height
        source:  { if (!cached) "qrc:/img-download"; else "qrc:/img-replay" }
        anchors.centerIn: parent
    }

    Video {
        id: player
        autoPlay: true
        fillMode: VideoOutput.Stretch
        enabled: false
        visible: false
        anchors.fill: parent
        onPlaybackStateChanged: {
            if (player.playbackState === MediaPlayer.StoppedState) {
                player.enabled = false
                player.visible = false
                icon.source = "qrc:/img-replay"
                icon.visible = true
            }
        }
    }

    Timer {
        id: delay
        interval: 50
        onTriggered: {
            if (cached) {
                player.source = "file://"+root.tmp+target
                player.enabled = true
                player.visible = true
            } else {
                cpp.preview_handler(id_private, host, url, target, false, false)
                downloading = true
            }
        }
    }
    Timer {
        id: delay_error
        interval: 50
        onTriggered: { icon.source = "qrc:/img_err" }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (!downloading) {
                if (player.playbackState === MediaPlayer.PlayingState) {
                    player.pause()
                } else if (player.playbackState === MediaPlayer.PausedState) {
                    player.play()
               } else if (player.playbackState === MediaPlayer.StoppedState) {
                    icon.visible = false
                    player.enabled = true
                    player.visible = true
                    player.play()
                }
            }
            view.forceActiveFocus()
        }
    }

    function abort() {
        if ((player.playbackState === MediaPlayer.PlayingState) || (player.playbackState === MediaPlayer.PausedState)) {
            player.stop()
        } else if (downloading) {
            icon.visible = false
            downloading = false
            cpp.preview_handler(id_private, "", "", "", false, true)
        }
        if (quit) {
            var sts = viewer.previewStatus.indexOf(0)
            if (sts !== -1) {
                viewer.previewStatus.splice(sts, 1);
                if (viewer.previewStatus.indexOf(0) === -1)
                    cpp.quit()
            }
        }
    }

    onQuitChanged: abort()

    Connections {
        target: cpp
        onSig_ret_preview: {
            if (id === id_private) {
                downloading = false
                if (success) {
                    icon.visible = false
                    root.cache_preview.push(id_cache)
                    cached = true
                    delay.start()
                } else {
                    delay_error.start()
                }
            }
        }
    }

    Component.onCompleted: delay.start()
    Component.onDestruction: { if (!quit) abort() }
}
// Tabs hechos.
