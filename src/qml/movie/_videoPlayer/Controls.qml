import QtQuick 2.4
import QtMultimedia 5.4

Item {
    id: controls

    Timer {
        id: delayHideControls
        running: !videoPlayer.sandbox
        interval: 4000
        onTriggered: {
            if ((mouseArea.mouseX < (parent.width-(parent.width/9.6))) && (mouseArea.mouseY < (parent.height-(parent.height/54))))
                videoPlayer.state = "hide_controls"
            else
                restart()
        }
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onPositionChanged: {
            if (videoPlayer.sideBoxMargin !== 0) {
                videoPlayer.state = "show_controls"
                delayHideControls.start()
            }
        }
        onClicked: playback(true)
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Space) || (event.key === Qt.Key_MediaPlay) ||
            (event.key === Qt.Key_MediaPause) || (event.key === Qt.Key_MediaStop)) {
            playback(true)
            event.accepted = true
        } else if (event.key === Qt.Key_Right) {
            playback(false, 10000)
            event.accepted = true
        } else if (event.key === Qt.Key_Left) {
            playback(false, -10000)
            event.accepted = true
        } else if ((event.key === Qt.Key_Up) || (event.key === Qt.Key_VolumeUp)) {
            volume(false, 0.1)
            event.accepted = true
        } else if ((event.key === Qt.Key_Down) || (event.key === Qt.Key_VolumeDown)) {
            volume(false, -0.1)
            event.accepted = true
        } else if ((event.key === Qt.Key_M) || (event.key === Qt.Key_VolumeMute)) {
            volume(true)
            event.accepted = true
        } else if (event.key === Qt.Key_Escape) {
            cpp.torrent_handler("", 0, true)
            movie.state = "hide"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }
    
    function playback(state, offset) {
        if (!videoPlayer.standby) {
            if (state) {
                if ((player.playbackState === MediaPlayer.PausedState) || (player.playbackState === MediaPlayer.StoppedState))
                    player.play()
                else
                    player.pause()
            } else {
                player.seek(player.position+offset)
            }
        }
    }
    function volume(mute, offset) {
        if (mute) {
            player.muted = !player.muted
        } else {
            if ((offset > 0 && player.volume < 1.0) || (offset < 0 && player.volume > 0.0)) {
                player.muted = false
                player.volume = player.volume+offset
            }
        }
    }
}
// Tabs hechos.
