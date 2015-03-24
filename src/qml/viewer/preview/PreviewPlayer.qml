import QtQuick 2.4
import QtMultimedia 5.4

Item {
    id: previewPlayer

    property bool cached
    property bool ready
    property bool downloading
    property bool quit: root.stop_preview_quit
    property string host
    property string url
    property string target: id_movie+"p.mp4"
    property int id_private
    property int id_movie

    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: width
        sourceSize.height: height
        opacity: 0.5
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
        onStatusChanged: {
            if ((player.status === 7) && ((player.position+1) !== player.duration)) { // FIX: Hacer que no se vuelva a cumplir al poner play nuevamente.
                player.seek(player.position-100)
                player.pause()
            }
        }
    }

    Timer {
        id: delay
        interval: 50
        onTriggered: {
            if (cached || ready) {
                player.source = "file://"+root.tmp+target
                player.enabled = true
                player.visible = true
            } else {
                cpp.preview_handler(id_private, host, url
                    , target, false, false, false, false)
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
            if (cached || ready) {
                if (player.playbackState === MediaPlayer.PlayingState) {
                    player.pause()
                } else if (player.playbackState === MediaPlayer.PausedState) {
                    player.seek(player.position) // TODO: Posiblemente no sea necesario.
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
        if ((player.playbackState === MediaPlayer.PlayingState)
            || (player.playbackState === MediaPlayer.PausedState))
            player.stop()
        if (downloading) {
            downloading = false
            cpp.preview_handler(id_private, "", "", ""
                , false, false, false, true)
        }
        if (quit) {
            viewer.n_previews -= 1
            if (viewer.n_previews === 0)
                cpp.quit()
        }
    }

    onQuitChanged: abort()

    Connections {
        target: cpp
        onSig_ret_stream: {
            if (id === id_private) {
                if (ready) {
                    previewPlayer.ready = true
                    icon.visible = false
                    delay.start()
                } else if (success) {
                    downloading = false
                    root.cache_preview.push(id_movie)
                    cached = true
                } else if (error) {
                    downloading = false
                    delay_error.start()
                }
            }
        }
    }

    Component.onCompleted: delay.start()
    Component.onDestruction: { if (!quit) abort() }
}
// Tabs hechos.
