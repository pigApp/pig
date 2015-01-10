import QtQuick 2.3

Item {
    id: scenesButton

    property alias totalScenes: repeater.model

    Grid {
        spacing: screen.width/192
        columns: 2
        anchors.top: parent.top
        anchors.left: parent.left
        Repeater {
            id: repeater
            delegate:
            Image {
                id: icon
                width: screen.width/60
                height: screen.height/42.63
                opacity: 0.1
                sourceSize.width: screen.width/60
                sourceSize.height: screen.height/42.63
                source: "qrc:/img-open"
                property bool viewed
                MouseArea {
                    hoverEnabled: true
                    onEntered: { if (!icon.viewed) icon.state = "in" }
                    onHoveredChanged: { if (!icon.viewed) icon.state = "out" }
                    onClicked: {
                        icon.viewed = true
                        screen.state = "show_torrent"
                        root.signal_qml_torrent_handler(urlTorrent, index+1, false)
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
                        NumberAnimation { target: icon; easing.amplitude: 1.7; properties: "opacity"; to: 0.1; duration: 100; easing.type: Easing.OutQuart }
                    }
                ]
            }
        }
    }
}
// Tabs hechos.
