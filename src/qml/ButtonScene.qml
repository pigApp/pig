import QtQuick 2.3

Rectangle {
    id: buttonScenne
    width: screen.width/22.58
    height: screen.height/12.70
    color: "transparent"

    property bool enter
    property string magnet
    property int scene
    property int size: screen.height/12.70

    Image {
        id: icon
        width: size
        height: size
        source: "/resources/images/output/open.png"
        smooth: true
        anchors.centerIn: parent
    }
    MouseArea {
        hoverEnabled: true
        onEntered: { enter = true }
        onHoveredChanged: { enter = false }
        onClicked: {
            if (!showTorrentInformation) {
                showTorrentInformation = true
                root.torrent_handler_qml_signal(magnet, scene, false)
            }
        }
        anchors.fill: parent
    }
    onEnterChanged: {
        if (enter) {
            inEffect.start()
        } else {
            inEffect.stop()
            outEffect.start()
        }
    }

    NumberAnimation { id: inEffect; target: buttonScenne; properties: "size"; to: screen.height/12; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outEffect; target: buttonScenne; properties: "size"; to: screen.height/12.70; duration: 300; easing.type: Easing.InOutQuart }
}
// Tabs hechos.
