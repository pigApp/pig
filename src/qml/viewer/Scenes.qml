import QtQuick 2.4

Item {
    id: btnScenes

    property alias totalScenes: repeater.model

    Row {
        spacing: screen.width/192
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Repeater {
            id: repeater
            delegate: Image {
                id: icon
                width: screen.width/25.26
                height: screen.height/14.21
                sourceSize.width: width
                sourceSize.height: height
                source: "qrc:/img-scene"
                property bool viewed
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        icon.viewed = true
                        icon.source = "qrc:/img-scene-off"
                        screen.state = "show_movie"
                        cpp.torrent_handler(urlTorrent, index+1, false)
                    }
                }
                Text {
                    id: label
                    text: index+1
                    color: "white"
                    font.family: fontGlobal.name
                    font.bold: true
                    font.pixelSize: screen.height/30
                    anchors.centerIn: parent
                    anchors.horizontalCenterOffset: -screen.width/2560
                }
            }
        }
    }
}
// Tabs hechos.
