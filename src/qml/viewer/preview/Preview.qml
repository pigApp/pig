import QtQuick 2.4

Item {
    id: preview

    property bool downaloaded

    /*
    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: 33
        sourceSize.height: 33
        source: "qrc:/img-play"
        anchors.centerIn: parent
    }
    */

    MouseArea {
        hoverEnabled: true
        onEntered: { preview.state = "in" }
        onHoveredChanged: { preview.state = "out" }
        onClicked: {
            if (!preview.downaloaded) {
                preview.downaloaded = true
                previewPlayer_loader.source = "PreviewPlayer.qml"
                previewPlayer_loader.active = true
            }
        }
        anchors.fill: parent
    }

    Loader {
        id: previewPlayer_loader
        active: false
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
        onStatusChanged: {
            if (status === Loader.Ready) {
                previewPlayer_loader.item.host = hostPreview
                previewPlayer_loader.item.url = urlPreview
                previewPlayer_loader.item.target = filePreview
                previewPlayer_loader.item.id = idPreview
            }
        }
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
            //NumberAnimation { target: viewerHandler; easing.amplitude: 1.7; properties: "boardOpacity"; to: 1.0; duration: 300; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "out"
            //NumberAnimation { target: viewerHandler; easing.amplitude: 1.7; properties: "boardOpacity"; to: 1.0; duration: 300; easing.type: Easing.OutQuart }
        }
    ]
}
// Tabs hechos.
