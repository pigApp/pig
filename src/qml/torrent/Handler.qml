import QtQuick 2.3

Rectangle {
    id: handler
    x: screen.width
    width: screen.width
    height: screen.height
    color: "black"

    property string sandboxX: root.sandbox

    Loader {
        id: dataDownload_loader
        source: "DataDownload.qml"
        active: false
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
    }

    Loader {
        id: videoPlayer_loader
        active: false
        asynchronous: true
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
            NumberAnimation { target: handler; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
            PropertyAction { target: dataDownload_loader; property: "active"; value: true }
        },
        Transition {
            to: "hide"
            NumberAnimation { target: handler; easing.amplitude: 1.7; properties: "x"; to: root.screenWidth; duration: 1100; easing.type: Easing.OutQuart }
        }
    ]

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            root.signal_qml_torrent_handler("", 0, true)
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }


    Timer {
        id: test
        running: false
        repeat: true
        interval: 4000
        onTriggered: {
            videoPlayer_loader.visible = true
            dataDownload_loader.source = ""
            dataDownload_loader.active = false
        }
    }

    onSandboxXChanged: {
        if (sandboxX !== "") {
            dataDownload_loader.item.state = "show_sandbox"
            videoPlayer_loader.source = "videoplayer/VideoPlayer.qml"
            videoPlayer_loader.active = true
            test.start()
        }
    }

    Component.onCompleted: { handler.state = "show" }
}
