import QtQuick 2.4

Item {
    id: scenesButtons

    property alias totalScenes: repeater.model

    Row {
        spacing: screen.width/192
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Repeater {
            id: repeater
            delegate:
            Image {
                id: icon
                width: screen.width/29.53
                height: screen.height/15.42
                sourceSize.width: icon.width
                sourceSize.height: icon.height
                source: "qrc:/img-scene"
                opacity: 0.5
                property bool viewed
                MouseArea {
                    hoverEnabled: true
                    onEntered: { if (!icon.viewed) icon.state = "in" }
                    onHoveredChanged: { if (!icon.viewed) icon.state = "out" }
                    onClicked: {
                        icon.viewed = true
                        screen.state = "show_torrent"
                        root.signal_qml_torrent_handler(urlTorrent, index, false)
                    }
                    anchors.fill: parent
                }
                states: [
                    State {
                        name: "in"
                    },
                    State {
                        name: "out"
                    }
                ]
                transitions: [
                    Transition {
                        to: "in"
                        NumberAnimation { target: icon; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 100; easing.type: Easing.OutQuart }
                    },
                    Transition {
                        to: "out"
                        NumberAnimation { target: icon; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
                    }
                ]
            }
        }
    }
}
// Tabs hechos.
