import QtQuick 2.3

Rectangle {
    id: buttonScenne
    width: screen.width/22.58
    height: screen.height/12.70
    color: "transparent"

    property string magnet
    property int scene
    property int size: screen.height/12.70

    Image {
        id: icon
        width: size
        height: size
        source: "/resources/images/viewer/open.png"
        smooth: true
        anchors.centerIn: parent
    }
    MouseArea {
        hoverEnabled: true
        onEntered: inEffect.start()
        onHoveredChanged: outEffect.start()
        onClicked: {
            if (!showTorrentInformation) {
                showTorrentInformation = true
                root.signal_qml_torrent_handler(magnet, scene, false)
            }
        }
        anchors.fill: parent
    }

    NumberAnimation { id: inEffect; target: buttonScenne; properties: "size"; to: screen.height/12; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outEffect; target: buttonScenne; properties: "size"; to: screen.height/12.70; duration: 300; easing.type: Easing.InOutQuart }
}
// Tabs hechos.
