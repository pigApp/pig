import QtQuick 2.4

Item {
    id: torrentHandler
    x: root.xa
    width: screen.width
    height: screen.height

    property string videoPath: root.videoPath
    property string sandboxStatus: ""

    Loader {
        id: loader_status
        active: false
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
    }
    Loader {
        id: loader_videoPlayer
        active: false
        asynchronous: true
        focus: loader_videoPlayer.visible
        visible: false
        anchors.fill: parent
    }

    states: [
        State {
            name: "show"
        },
        State {
            name: "hide"
        }
    ]
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {
                PropertyAction { target: loader_status; property: "source"; value: "Status.qml" }
                PropertyAction { target: loader_status; property: "active"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 1; duration: 900; easing.type: Easing.InSine }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: loader_root_b; property: "visible"; value: false }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: loader_root_b; property: "visible"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.5; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                 }
                PropertyAction { target: screen; property: "state"; value: "hide_torrent" }
            }
        }
    ]

    onVideoPathChanged: {
        if (videoPath !== "") {
            loader_videoPlayer.source = "VideoPlayer.qml"
            loader_videoPlayer.active = true
        }
    }
    onSandboxStatusChanged:  {
        if (sandboxStatus === "SUCCESS") {
            loader_videoPlayer.visible = true
            loader_status.source = ""
            loader_status.active = false
        }
    }

    Component.onCompleted: { torrentHandler.state = "show" }

    Component.onDestruction: {
        loader_status.source = ""
        loader_videoPlayer.source = ""
    }
}
// Tabs hechos.
