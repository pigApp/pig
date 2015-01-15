import QtQuick 2.4
import "../global/"

Item {
    id: quality

    Image {
        id: gridLayer
        width: parent.width
        height: parent.height
        sourceSize.width: 455
        sourceSize.height: 50
        source: "qrc:/img-grid-small"
    }

    Row {
        spacing: screen.width/192
        anchors.left: parent.left
        anchors.leftMargin: screen.width/384
        anchors.verticalCenter: parent.verticalCenter
        Button {
            id: sd
            width: screen.width/34.90
            height: screen.height/30.85
            label: "SD"
            labelColor: { if (root.quality === "SD") "black"; else "white" }
            labelLeftMargin: screen.width/384
            MouseArea {
                onClicked: { if (root.quality === "SD") root.quality = ""; else root.quality = "SD" }
                anchors.fill: parent
            }
        }
        Button {
            id: hd
            width: screen.width/33.68
            height: screen.height/30.85
            label: "HD"
            labelColor: { if (root.quality === "HD") "black"; else "white" }
            MouseArea {
                onClicked: { if (root.quality === "HD") root.quality = ""; else root.quality = "HD" }
                anchors.fill: parent
            }
        }
        Button {
            id: fhd
            width: screen.width/24
            height: screen.height/30.85
            label: "FHD"
            labelColor: { if (root.quality === "FHD") "black"; else "white" }
            MouseArea {
                onClicked: { if (root.quality === "FHD") root.quality = ""; else root.quality = "FHD" }
                anchors.fill: parent
            }
        }
        Button {
            id: full
            width: screen.width/36.92
            height: screen.height/30.85
            label: "FM"
            labelColor: { if (root.full === "FULL") "black"; else "white" }
            MouseArea {
                onClicked: { if (root.full === "FULL") root.full = ""; else root.full= "FULL" }
                anchors.fill: parent
            }
        }
    }
}
// Tabs hechos.
