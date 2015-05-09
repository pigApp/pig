import QtQuick 2.4

Item {
    id: dates

    property alias totalScenes: repeaterIcons.model

    Column {
        spacing: -screen.height/72
        anchors.top: parent.top
        anchors.left: parent.left
        Text {
            id: labelCategories
            text: categories
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
        }
        Text {
            id: labelQuality
            text: quality
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/23
        }
        Row {
            spacing: screen.width/128
            Text {
                id: labelFullMovie
                text: "FULL MOVIE"
                color: { if (full === "1") "white"; else "gray" }
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelTime
                text: time
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Row {
            spacing: screen.width/128
            Text {
                id: labelScenes
                text: "SCENES"
                color: "white"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/23
            }
            Row {
                spacing: screen.width/192
                anchors.verticalCenter: labelScenes.verticalCenter
                anchors.verticalCenterOffset: screen.height/360
                Repeater {
                    id: repeaterIcons
                    delegate: Image {
                        id: icon
                        property bool viewed
                        width: screen.width/58.18
                        height: screen.height/32.72
                        sourceSize.width: width
                        sourceSize.height: height
                        source: "qrc:/img-scene"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                icon.viewed = true
                                icon.source = "qrc:/img-scene-off"
                                screen.state = "show_movie"
                                cpp.torrent_handler(hostTorrent, urlTorrent
                                    , id_movie+"t.torrent", index+1, false)
                            }
                        }
                    }
                }
            }
        }
        Row {
            Text {
                id: labelCurrentMovie
                text: currentMovie
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
            Text {
                text: "·"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelTotalMovies
                text: root.n_movies
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
    }
}
// Tabs hechos.
