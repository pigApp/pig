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
        width: parent.width/3.85
        height: parent.height
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
                x: parent.width/10
                y: parent.height/6
                spacing: parent.height/540
                Text {
                    id: binary
                    text: "NEWS PIG "+binaryVersion+"."+release
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/36
                }
                Text {
                    text: binaryNews
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/54
                }
                Text {
                    id: database
                    text: "NEWS DATABASE "+databaseVersion
                    color: "white"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/36
                }
                Text {
                    text: databaseNews
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/54
                }
            }
        }
        Button {
            id: ok
            width: parent.width/13.4
            height: parent.height/54
            label: "OK"
            labelColor: "white"
            labelBold: true
            labelSize: parent.height/36
            labelInColor: "white"
            labelOutColor: "white"
            anchors.right: parent.right
            anchors.rightMargin: parent.width/24.8
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height/133
            onClicked: {
                root.news = false
                loader.source = ""
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: { news.forceActiveFocus() }
}
