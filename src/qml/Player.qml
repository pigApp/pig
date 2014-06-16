import QtQuick 2.2
import QtMultimedia 5.0

Item {
    id: player

    Video {
        id: video
        autoLoad: true
        autoPlay: true
        source: "qrc:/small.mp4"
        anchors.fill: parent
    }
}
