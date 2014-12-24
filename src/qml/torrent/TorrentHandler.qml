import QtQuick 2.3

Item {
    id: torrentHandler
    x: root.xA
    width: screen.width
    height: screen.height

    property string videoFilePath: root.videoFilePath
    property string sandboxStatus: ""

    Loader {
        id: downloadStatus_loader
        active: false
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
    }
    Loader {
        id: videoPlayer_loader
        active: false
        asynchronous: true
        focus: videoPlayer_loader.visible
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
                PropertyAction { target: downloadStatus_loader; property: "source"; value: "DownloadStatus.qml" }
                PropertyAction { target: downloadStatus_loader; property: "active"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 1; duration: 900; easing.type: Easing.InSine }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: root_loader_B; property: "visible"; value: false }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: root_loader_B; property: "visible"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.5; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                 }
                PropertyAction { target: screen; property: "state"; value: "hide_torrent" }
            }
        }
    ]

    onVideoFilePathChanged: {
        if (videoFilePath !== "") {
            videoPlayer_loader.source = "VideoPlayer.qml"
            videoPlayer_loader.active = true
        }
    }
    onSandboxStatusChanged:  {
        if (sandboxStatus === "success") {
            videoPlayer_loader.visible = true
            downloadStatus_loader.source = ""
            downloadStatus_loader.active = false
        }
    }

    Component.onCompleted: { torrentHandler.state = "show" }

    Component.onDestruction: {
        downloadStatus_loader.source = ""
        videoPlayer_loader.source = ""
    }
}
