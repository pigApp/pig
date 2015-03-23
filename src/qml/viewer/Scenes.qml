import QtQuick 2.4

Item {
    id: btnScenes

    property alias totalScenes: repeater.model

    Row {
        spacing: screen.width/192
        anchors.top: parent.top
        anchors.left: parent.left
        Text {
            id: labelScenes
            text: "SCENES"
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
            anchors.top: parent.top
            anchors.topMargin: screen.height/135
        }
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
                        cpp.torrent_handler(hostTorrent, urlTorrent, id_movie+"t.torrent", index+1, false)
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
