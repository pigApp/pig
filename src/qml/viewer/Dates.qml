import QtQuick 2.4

Item {
    id: dates

    Column {
        spacing: -screen.height/72
        anchors.top: parent.top
        anchors.left: parent.left
        Text {
            id: labelCategories
            text: categories
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: labelQuality
            text: quality
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: labelTime
            text: time
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: labelFull
            text: "FULL"
            color: { if (full === "1") "white"; else Qt.rgba(1, 1, 1, 0.2) }
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
    }
}
// Tabs hechos.
