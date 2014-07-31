import QtQuick 2.2

Item {
    id: news

    Rectangle {
      id: backgroundLayer
      color: Qt.rgba(0.5, 0.5, 0.5, 0.7)
      anchors.fill: parent
    }

    Rectangle {
        id: layer
        width: screen.width/3.85
        height: screen.height
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.centerIn: parent
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
                y: 40
                spacing: 2
                Text {
                    id: binary
                    text: "NEWS PIG "+binaryVersion+"."+release
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: 30
                }
                Text {
                    text: binaryNews
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: 20
                }
                Text {
                    id: database
                    text: "NEWS DATABASE "+databaseVersion
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: 30
                }
                Text {
                    text: databaseNews
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: 20
                }
            }
        }
        Button {
            id: ok
            width: 37
            height: 20
            label: "OK"
            labelColor: "white"
            labelBold: true
            labelSize: 30
            labelInColor: "white"
            labelOutColor: "white"
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            onClicked: {
                root.news = false
                loader.source = ""
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: { news.forceActiveFocus() }
}
