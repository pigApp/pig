import QtQuick 2.4
import "layer_components/"

Item {
    id: layer

    Column {
        id: titleCastColumn
        spacing: -screen.height/72
        visible: !root.onShowTorrent
        anchors.horizontalCenter: preview.horizontalCenter
        anchors.bottom: preview.top
        anchors.bottomMargin: 20//screen.height/21.6
        Text {
            id: titleLabel
            text: title
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/10
        }
        Text {
            id: castLabel
            text: cast
            color: Qt.rgba(1, 1, 1, 0.1)
            font.family: customFont.name
            font.letterSpacing: screen.width/480
            font.wordSpacing: -screen.width/384
            font.pixelSize: screen.height/38
            anchors.horizontalCenter: titleLabel.horizontalCenter
        }
    }
    Preview {
        id: preview
        width: screen.width/3
        height: screen.height/2.25
        visible: !root.onShowTorrent
        anchors.centerIn: parent
    }
    Dates {
        id: dates
        anchors.top: preview.bottom
        anchors.topMargin: 20//
        anchors.horizontalCenter: preview.horizontalCenter
    }
    Scenes {
        id: scenesButtons
        totalScenes: scenes
        anchors.top: dates.bottom
        anchors.topMargin: 45//
        anchors.horizontalCenter: dates.horizontalCenter
    }
}
// Tabs hechos.
