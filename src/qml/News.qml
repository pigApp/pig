import QtQuick 2.0

Item {
    id: news

    Column {
        id: newColumn
        spacing: 2
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 100

        Text {
            id: binary
            text: "NEWS PIG "+binaryVersion+"."+release
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 20
        }
        Text {
            text: binaryNews
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
        }

        Text {
            id: database
            text: "NEWS DATABASE "+databaseVersion
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 20
        }
        Text {
            text: databaseNews
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
        }
    }
}
