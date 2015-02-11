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
            color: "#FA6900"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: labelQuality
            text: { if (quality === "FD") "FHD"; else quality }
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
            color: { if (full === "1") "white"; else "red" }
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
        }
        Row {
            Text {
                id: labelCurrentFilm
                text: currentFilm
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
            Text {
                text: "·"
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
            Text {
                id: labelTotalFilms
                text: root.n_films
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
    }
}
// Tabs hechos.
