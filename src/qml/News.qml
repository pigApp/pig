import QtQuick 2.0

Item {
    id: news

    Column {
        id: newColumn
        spacing: 7
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 100

        Text {
            id: binaryNewsLabel
            text: "NEWS PIG "+binaryVersion+"."+release
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 20
        }
        Text {
            id: binaryNews
            text: "+ Support for custom torrent files.\n+ Support for custom torrent files.\n- Preview http films."
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
        }

        Text {
            id: databaseNewsLabel
            text: "NEWS DATABASE "+databaseVersion
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 20
        }
        Text {
            id: databaseNews
            text: "+ Support for custom torrent files.\n+ Support for custom torrent files."
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
        }
    }
}
