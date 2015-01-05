import QtQuick 2.3

Rectangle {
    id: preview
    color: Qt.rgba(0, 0, 0, 0.05)

    Column {
        spacing: screen.height/360
        anchors.centerIn: parent
        anchors.verticalCenterOffset: screen.height/135
        Image {
            id: icon
            width: screen.width/60
            height: screen.height/33.75
            sourceSize.width: screen.width/60
            sourceSize.height: screen.height/33.75
            source: "qrc:/img-play"
            opacity: 0.2
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id: label
            width: screen.width/27.7
            height: screen.height/72
            text: "PREVIEW"
            color: Qt.rgba(1, 1, 1, 0.1)
            font.family: globalFont.name
            font.pixelSize: screen.height/54
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea {
        onClicked: {
            if (icon.visible) {
                icon.visible = false
                label.visible = false
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
