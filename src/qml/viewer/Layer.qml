import QtQuick 2.3
import "layer_components/"

Item {
    id: layer

    Column {
        id: titleCastColumn
        spacing: -screen.height/108
        visible: !root.onShowTorrent
        anchors.horizontalCenter: preview.horizontalCenter
        anchors.bottom: preview.top
        anchors.bottomMargin: screen.height/21.6
        Text {
            id: titleLabel
            text: title
            color: "white"
            font.family: fancyFont.name
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
    Dates {
        id: dates
        anchors.left: preview.left
        anchors.leftMargin: -screen.width/11.29
        anchors.bottom: preview.verticalCenter
        anchors.bottomMargin: -screen.height/13.05
    }
    Scenes {
        id: scenesButton
        totalScenes: scenes
        anchors.top: preview.verticalCenter
        anchors.topMargin: screen.height/11.25
        anchors.left: preview.left
        anchors.leftMargin: -screen.width/11.29
    }
    Preview {
        id: preview
        width: screen.width/3
        height: screen.height/2.25
        visible: !root.onShowTorrent
        anchors.centerIn: parent
    }
}
// Tabs hechos.
