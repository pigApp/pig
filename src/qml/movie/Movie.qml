import QtQuick 2.4

Item {
    id: movie
    x: root.xa
    width: screen.width
    height: screen.height

    property string movie_file: root.movie_file
    property string sandboxStatus: ""

    Loader {
        id: loader_torrentStatus
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
                PropertyAction { target: loader_torrentStatus; property: "source"; value: "torrentStatus.qml" }
                PropertyAction { target: loader_torrentStatus; property: "active"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: loader_root_b; property: "visible"; value: false }
                PropertyAction { target: loader_root_b; property: "enabled"; value: false }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: loader_root_b; property: "visible"; value: true }
                PropertyAction { target: loader_root_b; property: "enabled"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                 }
                PropertyAction { target: screen; property: "state"; value: "hide_movie" }
            }
        }
    ]

    onMovie_fileChanged:  {
        if (movie_file !== "") {
            loader_videoPlayer.source = "VideoPlayer.qml"
            loader_videoPlayer.active = true
        }
    }
    onSandboxStatusChanged:  {
        if (sandboxStatus === "SUCCESS") {
            loader_videoPlayer.visible = true
            loader_torrentStatus.source = ""
            loader_torrentStatus.active = false
        }
    }

    Component.onCompleted: { movie.state = "show" }
    Component.onDestruction: {
        loader_torrentStatus.source = ""
        loader_videoPlayer.source = ""
    }
}
// Tabs hechos.
