import QtQuick 2.3

Rectangle {
    id: news
    color: "black"
    anchors.fill: parent

    Column {
        id: newsColumn
        spacing: parent.height/108
        anchors.centerIn: parent
        Text {
            id: binaryLabel
            text: "PIG "+root.binary+"."+root.release
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
            textFormat: Text.RichText
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            id: databaseLabel
            text: "DB "+root.database
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
            textFormat: Text.RichText
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Return) {
            screen.state = "hideNews"
            event.accepted = true;
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: news.forceActiveFocus()
}
// Tabs hechos.
