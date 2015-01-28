import QtQuick 2.4

Item {
    id: preview

    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: icon.width
        sourceSize.height: icon.height
        source: "qrc:/img-play"
        anchors.centerIn: parent
    }

    MouseArea {
        onClicked: {
            if (icon.visible) {
                icon.visible = false
                loader_previewPlayer.source = "PreviewPlayer.qml"
                loader_previewPlayer.active = true
            }
        }
        anchors.fill: parent
    }

    Loader {
        id: loader_previewPlayer
        active: false
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
        onStatusChanged: {
            if (status === Loader.Ready) {
                loader_previewPlayer.item.host = hostPreview
                loader_previewPlayer.item.url = urlPreview
                loader_previewPlayer.item.target = filePreview
                loader_previewPlayer.item.id = idPreview
            }
        }
    }
}
// Tabs hechos.
