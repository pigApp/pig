import QtQuick 2.3
import QtMultimedia 5.0

Item {
    id: constrols

    Timer {
        id: hideControlsDelay
        running: !videoPlayerHandler.sandbox
        repeat: false
        interval: 4000
        onTriggered: {
            if ((mouseArea.mouseX < (parent.width-200)) && (mouseArea.mouseY < (parent.height-20)))
                videoPlayerHandler.state = "hideControls"
            else
                hideControlsDelay.restart()
        }
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        onClicked: playback(true)
        onPositionChanged: {
            if (videoPlayerHandler.sideBoxMargin !== 0) {
                videoPlayerHandler.state = "showControls"
                hideControlsDelay.start()
            }
        }
        anchors.fill: parent
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Space || event.key === Qt.Key_MediaPlay ||
            event.key === Qt.Key_MediaPause || event.key === Qt.Key_MediaStop) {
            playback(true)
            event.accepted = true;
        } else if (event.key === Qt.Key_Right) {
            playback(false, 10000)
            event.accepted = true;
        } else if (event.key === Qt.Key_Left) {
            playback(false, -10000)
            event.accepted = true;
        } else if (event.key === Qt.Key_Up || event.key === Qt.Key_VolumeUp) {
            volume(false, 0.1)
            event.accepted = true;
        } else if (event.key === Qt.Key_Down || event.key === Qt.Key_VolumeDown) {
            volume(false, -0.1)
            event.accepted = true;
        } else if (event.key === Qt.Key_M || event.key === Qt.Key_VolumeMute) {
            volume(true)
            event.accepted = true;
        } else if (event.key === Qt.Key_Escape) {
            //screen.state = "show_finder"
            console.log("//// ESC")
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }
    
    function playback(state, offset) {
        if (!videoPlayerHandler.standby) {
            if (state) {
                if (player.playbackState === MediaPlayer.PausedState || player.playbackState === MediaPlayer.StoppedState)
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
