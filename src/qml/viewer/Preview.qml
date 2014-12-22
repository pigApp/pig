import QtQuick 2.3

Rectangle {
    id: preview
    color: Qt.rgba(1, 1, 1, 0.03)

    Image {
        id: icon
        width: screen.width/60
        height: screen.height/33.75
        sourceSize.width: screen.width/60
        sourceSize.height: screen.height/33.75
        source: "qrc:/img-play"
        opacity: 0.5
        anchors.centerIn: parent
    }

    MouseArea {
        onClicked: {
            if (icon.visible) {
                icon.visible = false
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
}
// Tabs hechos.
