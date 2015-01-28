import QtQuick 2.4
import QtMultimedia 5.4
import "videoplayer_components/"

Rectangle {
    id: videoPlayerHandler
    color: "black"

    property bool sandbox: true
    property bool standby
    property int sideBoxMargin: 0
    property int barsMargin: 0

    Video {
        id: player
        autoPlay: true
        volume: 0.8 
        fillMode: VideoOutput.PreserveAspectFit
        anchors.fill: parent
        onStatusChanged: {
            if (sandbox) {
                if ((player.status === MediaPlayer.Buffered) && !player.error) {
                    torrentHandler.sandboxStatus = "SUCCESS"
                    videoPlayerHandler.sandbox = false
                    controls.forceActiveFocus()
                } else if ((player.status === MediaPlayer.NoMedia) || (player.status === MediaPlayer.InvalidMedia)) {
                    torrentHandler.sandboxStatus = "FAIL"
                    // Timer recheck video start -10
                }
            } else {
                //...
            }
        }
    }
    Control {}

    SideBox {
        id: sideBox
        width: parent.width/9.6
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: sideBoxMargin
        anchors.verticalCenter: parent.verticalCenter
    }

    Bars {
        id: bars
        width: parent.width 
        height: parent.height/54
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: barsMargin
    } 

    Controls {
        id: controls
        anchors.fill: parent
    }

    states: [
        State {
            name: "show_controls"
        },
        State {
            name: "hide_controls"
        }
    ]
    transitions: [
        Transition {
            to: "show_controls"
            ParallelAnimation {
                NumberAnimation { target: videoPlayerHandler; easing.amplitude: 1.7; properties: "sideBoxMargin"; to: 0; duration: 500; easing.type: Easing.OutQuart }
                NumberAnimation { target: videoPlayerHandler; easing.amplitude: 1.7; properties: "barsMargin"; to: 0; duration: 200; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hide_controls"
            ParallelAnimation {
                NumberAnimation { target: videoPlayerHandler; easing.amplitude: 1.7; properties: "sideBoxMargin"; to: -screen.width/9.6; duration: 500; easing.type: Easing.OutQuart }
                NumberAnimation { target: videoPlayerHandler; easing.amplitude: 1.7; properties: "barsMargin"; to: -screen.height/54; duration: 500; easing.type: Easing.OutQuart }
            }
        }
    ]
    
    onStandbyChanged: { if (standby) player.pause(); else player.play() } 

    Component.onCompleted: { player.source = "file://"+root.videoPath }

    Component.onDestruction: { if ((player.playbackState === MediaPlayer.PlayingState) ||
                                   (player.playbackState === MediaPlayer.PausedState))
                                    player.stop() }
}
// Tabs hechos.
