import QtQuick 2.4
import QtMultimedia 5.4

import "_videoPlayer/"

Rectangle {
    id: videoPlayer
    color: "black"

    property bool sandbox: true
    property bool standby
    property int timeLeft: 9
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
                    movie.sandboxStatus = "SUCCESS"
                    videoPlayer.sandbox = false
                    controls.forceActiveFocus()
                } else if (((player.status === MediaPlayer.NoMedia)
                    || (player.status === MediaPlayer.InvalidMedia)
                    && (player.source !== ""))) {
                    player.source = ""
                    movie.sandboxStatus = "FAIL"
                }
            } else {
                //...
            }
        }
    }
    Timer {
        id: delaySandbox
        running: { movie.sandboxStatus === "FAIL" }
        repeat: true
        interval: 1000
        onTriggered: {
            if (timeLeft > 0) {
                timeLeft -= 1
            } else { 
                player.source = "file://"+root.movie_file_path
                timeLeft = 9
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
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "sideBoxMargin"
                    ; to: 0; duration: 500; easing.type: Easing.OutQuart }
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "barsMargin"
                    ; to: 0; duration: 200; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hide_controls"
            ParallelAnimation {
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "sideBoxMargin"
                    ; to: -screen.width/9.6; duration: 500; easing.type: Easing.OutQuart }
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "barsMargin"
                    ; to: -screen.height/54; duration: 500; easing.type: Easing.OutQuart }
            }
        }
    ]
    
    onStandbyChanged: { 
        if (!sandbox)
            if (standby) player.pause(); else player.play()
    } 

    Component.onCompleted: { player.source = "file://"+root.movie_file_path }
    Component.onDestruction: {
        if ((player.playbackState === MediaPlayer.PlayingState)
            || (player.playbackState === MediaPlayer.PausedState))
            player.stop()
    }
}
// Tabs hechos.
