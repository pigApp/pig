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
        spacing: screen.width/384
        anchors.left: parent.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: parent.verticalCenter
        Button {
            id: sd
            label: "SD"
            labelColor: { if (root.quality === "SD") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.quality === "SD") {
                        sd.lockColor = false
                        root.quality = ""
                    } else {
                        sd.lockColor = true
                        root.quality = "SD"
                    }
                }
                anchors.fill: parent
            }
        }
        Button {
            id: hd
            label: "HD"
            labelColor: { if (root.quality === "HD") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.quality === "HD") {
                        hd.lockColor = false
                        root.quality = ""
                    } else {
                        hd.lockColor = true
                        root.quality = "HD"
                    }
                }
                anchors.fill: parent
            }
        }
        Button {
            id: fhd
            label: "FHD"
            labelColor: { if (root.quality === "FHD") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.quality === "FHD") {
                        fhd.lockColor = false
                        root.quality = ""
                    } else {
                        fhd.lockColor = true
                        root.quality = "FHD"
                    }
                }
                anchors.fill: parent
            }
        }
        Button {
            id: full
            label: "FULLMOVIE"
            labelColor: { if (root.full === "1") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.full === "1") {
                        full.lockColor = false
                        root.full = ""
                    } else {
                        full.lockColor = true
                        root.full = "1"
                    }
                }
                anchors.fill: parent
            }
        }
    }

    Component.onCompleted: {
        if (root.quality === "SD")
            sd.lockColor = true
        if (root.quality === "HD")
            hd.lockColor = true
        if (root.quality === "FHD")
            fhd.lockColor = true
        if (root.full === "1")
            full.lockColor = true
    }
}
// Tabs hechos.
