import QtQuick 2.4

Item {
    id: btnScenes

    property alias totalScenes: repeater.model

    Column {
        spacing: screen.width/192
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Repeater {
            id: repeater
            delegate: Image {
                id: icon
                width: screen.width/29.53
                height: screen.height/27
                sourceSize.width: width
                sourceSize.height: height
                source: "qrc:/img-scene"

                property bool viewed

                MouseArea {
                    hoverEnabled: true
                    onEntered: { if (!icon.viewed) icon.state = "in" }
                    onHoveredChanged: { if (!icon.viewed) icon.state = "out" }
                    onClicked: {
                        icon.viewed = true
                        screen.state = "show_movie"
                        cpp.torrent_handler(urlTorrent, index, false)
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
                        NumberAnimation { target: icon; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
                    },
                    Transition {
                        to: "out"
                        NumberAnimation { target: icon; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 100; easing.type: Easing.OutQuart }
                    }
                ]
            }
        }
    }
}
// Tabs hechos.
