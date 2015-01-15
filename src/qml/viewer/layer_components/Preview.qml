import QtQuick 2.4

Item {
    id: preview

    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: 33
        sourceSize.height: 33
        source: "qrc:/img-play"
        opacity: 0.1
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
