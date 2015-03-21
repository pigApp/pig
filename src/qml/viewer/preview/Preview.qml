import QtQuick 2.4

Item {
    id: preview

    property bool cached

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
        anchors.fill: parent
        onClicked: {
            if (icon.visible) {
                icon.visible = false
                loader_previewPlayer.source = "PreviewPlayer.qml"
                loader_previewPlayer.active = true
            }
        }
    }

    Loader {
        id: loader_previewPlayer
        active: false
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
        onStatusChanged: {
            if (status === Loader.Ready) {
                loader_previewPlayer.item.id_private = id_private
                loader_previewPlayer.item.id_movie = id_movie
                loader_previewPlayer.item.host = hostPreview
                loader_previewPlayer.item.url = urlPreview
                loader_previewPlayer.item.cached = preview.cached
                viewer.n_previews += 1
            }
        }
    }

    Component.onCompleted: {
        if (root.cache_preview.indexOf(parseInt(id_movie)) !== -1)
            preview.cached = true
    }
}
// Tabs hechos.
