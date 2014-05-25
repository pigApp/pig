import QtQuick 2.1
import QtAV 1.3

Item {
    id: previewPlayer
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: 1.8

    property alias previewWidth: previewPlayer.width
    property alias previewHeight: previewPlayer.height
    property alias url: player.source

    signal kill()
    onKill: player.stop()

    Image {
        id: play
        source: "qrc:/images/player/playPreview.png"
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -3
        MouseArea {
            anchors.fill: parent
            onClicked: { if (player.playbackState === MediaPlayer.PlayingState) player.pause(); else player.play() }
        }
    }
    Text {
        id: label
        text: "PREVIEW"
        color: "white"
        font.family: pigFont.name
        font.pixelSize: 15
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 48
    }

    VideoOutput {
        id: videoOutput
        source: player
        opacity: 0
        fillMode: VideoOutput.Stretch
        anchors.fill: parent
    }
    AVPlayer {
        id: player
        source: url
        autoLoad: false
        onPlaying: {
            label.opacity = 0
            videoOutput.opacity = 1
        }
        onStopped: {
            videoOutput.opacity = 0
            play.opacity = 0.15
            label.text = ""
            label.anchors.verticalCenterOffset = 30
            label.opacity = 1
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (player.playbackState === MediaPlayer.PlayingState) {
                player.pause()
            } else {
                play.opacity = 0
                label.opacity = 0.15
                label.text = "LOADING..."
                label.anchors.verticalCenterOffset = 5
                player.play()
            }
        }
    }
}
// Espacios hecho.
