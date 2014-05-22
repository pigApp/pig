import QtQuick 2.1
import QtAV 1.3

Rectangle {
    id: layer
    width: 480
    height: 270
    color: Qt.rgba(0, 0, 0, 0.10)
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.horizontalCenterOffset: 43.5
    anchors.verticalCenter: parent.verticalCenter
                
    property alias url: player.source

    signal kill()
    onKill: player.stop()

    Image {
        id: play
        source: "qrc:/images/player/play.png"
        opacity: 0.10
        fillMode: Image.PreserveAspectCrop
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        MouseArea {
            anchors.fill: parent
            onClicked: { if (player.playbackState === MediaPlayer.PlayingState) player.pause(); else player.play() }
        }
    }
    Text {
        id: label
        text: "PREVIEW"
        color: Qt.rgba(1, 1, 1, 0.10)
        font.family: pigFont.name
        font.weight: Font.Light
        font.pixelSize: 11
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 30
    }

    VideoOutput {
        id: videoOutput
        source: player
        opacity: 0
        fillMode: VideoOutput.PreserveAspectFit
        anchors.fill: parent
    }
    AVPlayer {
        id: player
        source: url
        autoLoad: false
        onPlaying: {
            layer.color = Qt.rgba(0, 0, 0, 0)
            label.opacity = 0
            videoOutput.opacity = 1
        }
        onStopped: {
            videoOutput.opacity = 0
            layer.color = Qt.rgba(0, 0, 0, 0.10)
            play.opacity = 0.15
            label.text = "PREVIEW"
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
                label.text = "LOADING..."
                label.anchors.verticalCenterOffset = 5
                player.play()
            }
        }
    }
}
// Espacios hecho.
