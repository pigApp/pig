import QtQuick 2.3
import QtMultimedia 5.0

Rectangle {
    id: videoPlayer
    color: "black"
    anchors.fill: parent

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
                if (player.status === MediaPlayer.Buffered && !player.error) {
                    handler.sandboxStatus = "success"
                    videoPlayer.sandbox = false
                    controls.forceActiveFocus()
                } else if (player.status === MediaPlayer.NoMedia || player.status === MediaPlayer.InvalidMedia) {
                    handler.sandboxStatus = "fail"
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
        width: 200//
        height: parent.height/1.33
        anchors.right: parent.right
        anchors.rightMargin: sideBoxMargin
        anchors.verticalCenter: parent.verticalCenter
    }
    Bars {
        id: bars
        width: parent.width 
        height: 20//
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: barsMargin
    } 
    ControlsHandler {
        id: controls
        anchors.fill: parent
    }

    states: [
        State {
            name: "showControls"
        },
        State {
            name: "hideControls"
        }
    ]
    transitions: [
        Transition {
            to: "showControls"
            ParallelAnimation {
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "sideBoxMargin"; to: 0; duration: 500; easing.type: Easing.OutQuart }
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "barsMargin"; to: 0; duration: 200; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hideControls"
            ParallelAnimation {
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "sideBoxMargin"; to: -200; duration: 500; easing.type: Easing.OutQuart }
                NumberAnimation { target: videoPlayer; easing.amplitude: 1.7; properties: "barsMargin"; to: -20; duration: 500; easing.type: Easing.OutQuart }
            }
        }
    ]
    
    onStandbyChanged: { if (standby) player.pause(); else player.play() } 

    Component.onCompleted: { player.source = "file://"+root.videoFilePath }
}
// Tabs hechos.
