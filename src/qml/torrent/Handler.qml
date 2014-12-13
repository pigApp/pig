import QtQuick 2.3

Rectangle {
    id: handler
    x: root.xA
    width: screen.width
    height: screen.height
    color: "black"

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
            NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
            PropertyAction { target: downloadStatus_loader; property: "source"; value: "DownloadStatus.qml" }
            PropertyAction { target: downloadStatus_loader; property: "active"; value: true }
        },
        Transition {
            to: "hide"
            NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: root.screenWidth; duration: 1100; easing.type: Easing.OutQuart }
        }
    ]

    onVideoFilePathChanged: {
        if (videoFilePath !== "") {
            videoPlayer_loader.source = "videoplayer/VideoPlayer.qml"
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

    Component.onCompleted: { handler.state = "show" }
}
