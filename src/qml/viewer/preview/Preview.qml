import QtQuick 2.4

Item {
    id: preview

    property bool cached
    property int id_cache: id_film

    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: width
        sourceSize.height: height
        source: { if (!cached) "qrc:/img-play"; else "qrc:/img-replay" }
        opacity: 0.7
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
                loader_previewPlayer.item.id_private = id_preview
                loader_previewPlayer.item.id_cache = id_cache
                loader_previewPlayer.item.host = hostPreview
                loader_previewPlayer.item.url = urlPreview
                loader_previewPlayer.item.cached = preview.cached
                viewerHandler.previewStatus.push(0)
            }
        }
    }

    Component.onCompleted: { if (root.cache_preview.indexOf(id_cache) !== -1)  preview.cached = true }
}
// Tabs hechos.
