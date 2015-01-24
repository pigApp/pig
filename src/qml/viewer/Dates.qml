import QtQuick 2.4

Item {
    id: dates

    Column {
        spacing: -screen.height/72
        anchors.top: parent.top
        anchors.left: parent.left
        Text {
            id: categoriesLabel
            text: categories
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: qualityLabel
            text: quality
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: timeLabel
            text: time
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: fullLabel
            text: "FULL"
            color: { if (full === "1") "white"; else Qt.rgba(1, 1, 1, 0.2) }
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
    }
}
// Tabs hechos.
