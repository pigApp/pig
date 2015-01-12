import QtQuick 2.4

Item {
    id: dates

    Column {
        spacing: screen.height/216
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Text {
            id: categoriesLabel
            text: categories
            color: "white"
            font.family: customFont.name
            font.letterSpacing: screen.width/960
            font.pixelSize: screen.height/30
        }
        Text {
            id: qualityLabel
            text: quality
            color: "white"
            font.family: customFont.name
            font.letterSpacing: screen.width/960
            font.pixelSize: screen.height/30
        }
        Text {
            id: splitLabel
            text: "SPLIT"
            color: { if (scenes !== 1) "white"; else Qt.rgba(1, 1, 1, 0.1) }
            font.family: customFont.name
            font.letterSpacing: screen.width/960
            font.pixelSize: screen.height/30
        }
        Text {
            id: fullLabel
            text: "FULL"
            color: { if (full === "") "white"; else Qt.rgba(1, 1, 1, 0.1) }
            font.family: customFont.name
            font.letterSpacing: screen.width/960
            font.pixelSize: screen.height/30
        }
        Row {
            id: counterRow
            spacing: screen.width/480
            Text {
                id: currentFilmLabel
                text: viewerHandler.current_film
                color: { if (viewerHandler.location_block+1 !== root.block_films) "white"; else Qt.rgba(1, 1, 1, 0.5) }
                font.family: customFont.name
                font.pixelSize: screen.height/30
            }
            Text {
                id: totalFilmsLabel
                text: root.total_films
                color: { if (viewerHandler.current_film !== root.total_films) Qt.rgba(1, 1, 1, 0.5); else "white" }
                font.family: customFont.name
                font.letterSpacing: screen.width/960
                font.pixelSize: screen.height/30
            }
        }
    }
}
// Tabs hechos.
