import QtQuick 2.4

Item {
    id: movie
    x: root.xa
    width: screen.width
    height: screen.height

    property string movie_file_path: root.movie_file_path
    property string sandboxStatus: ""

    Loader {
        id: loader_torrentStatus
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
    }
    Loader {
        id: loader_videoPlayer
        asynchronous: true
        focus: loader_videoPlayer.visible
        enabled: false
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
                PropertyAction { target: loader_torrentStatus; property: "source"; value: "TorrentStatus.qml" }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"
                        ; to: -(screen.width+10); duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"
                        ; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: loader_root_b; property: "enabled"; value: false }
                PropertyAction { target: loader_root_b; property: "visible"; value: false }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: loader_root_b; property: "enabled"; value: true }
                PropertyAction { target: loader_root_b; property: "visible"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"
                        ; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"
                        ; to: screen.width+10; duration: 1100; easing.type: Easing.OutQuart }
                 }
                PropertyAction { target: screen; property: "state"; value: "hide_movie" }
            }
        }
    ]

    onMovie_file_pathChanged:  {
        if (movie_file_path !== "")
            loader_videoPlayer.source = "VideoPlayer.qml"
    }
    onSandboxStatusChanged:  {
        if (sandboxStatus === "SUCCESS") {
            loader_videoPlayer.enabled = true
            loader_videoPlayer.visible = true
            loader_torrentStatus.source = ""
        }
    }

    Component.onCompleted: { movie.state = "show" }
    Component.onDestruction: {
        loader_torrentStatus.source = ""
        loader_videoPlayer.source = ""
    }
}
// Tabs hechos.
