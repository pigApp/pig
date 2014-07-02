import QtQuick 2.0

Rectangle {
    id: layer
    x: screen.width-screen.width/3.55
    width: screen.width/3.55
    height: screen.height
    color: Qt.rgba(0, 0, 0, 0.9)
    z: 1
    Flickable {
        id: flickAreaNews
        contentWidth: newsColumn.width
        contentHeight: newsColumn.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        anchors.fill: parent
        Column {
            id: newsColumn
            x: 50
            y: 30
            spacing: 2
            Text {
                id: binary
                text: "NEWS PIG "+binaryVersion+"."+release
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 35
            }
            Text {
                text: binaryNews
                color: "white"
                font.family: pigFont.name
                font.pixelSize: 35
            }
            Text {
                id: database
                text: "NEWS DATABASE "+databaseVersion
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 35
            }
            Text {
                text: databaseNews
                color: "white"
                font.family: pigFont.name
                font.pixelSize: 35
            }
        }
    }
}
