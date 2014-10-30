import QtQuick 2.2

Item {
    id: news

    Rectangle {
        id: layer
        width: parent.width/4
        height: parent.height
        color: "black"
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
                anchors.left: parent.left
                anchors.leftMargin: parent.width/12
                anchors.top: parent.top
                anchors.topMargin: parent.height/43
                spacing: parent.height/216
                Text {
                    id: binary
                    text: "NEWS PIG "+binaryVersion+"."+release
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: binaryNews
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/54
                }
                Text {
                    id: database
                    text: "NEWS DATABASE "+databaseVersion
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: databaseNews
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/54
                }
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            screen.state = "hideNews"
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: { news.forceActiveFocus() }
}
