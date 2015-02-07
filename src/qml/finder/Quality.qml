import QtQuick 2.4
import "../global/"

Item {
    id: quality

    Image {
        id: layerGrid
        width: parent.width
        height: parent.height
        sourceSize.width: layerGrid.width
        sourceSize.height: layerGrid.height
        source: ""
        visible: false //
    }

    Row {
        spacing: screen.width/384
        anchors.left: parent.left
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: parent.verticalCenter
        Button {
            id: btnSd
            label: "SD"
            labelColor: { if (root.quality === "SD") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.quality === "SD") {
                        btnSd.lockColor = false
                        root.quality = ""
                    } else {
                        btnSd.lockColor = true
                        root.quality = "SD"
                    }
                }
                anchors.fill: parent
            }
        }
        Button {
            id: btnHd
            label: "HD"
            labelColor: { if (root.quality === "HD") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.quality === "HD") {
                        btnHd.lockColor = false
                        root.quality = ""
                    } else {
                        btnHd.lockColor = true
                        root.quality = "HD"
                    }
                }
                anchors.fill: parent
            }
        }
        Button {
            id: btnFd
            label: "FHD"
            labelColor: { if (root.quality === "FD") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.quality === "FD") {
                        btnFd.lockColor = false
                        root.quality = ""
                    } else {
                        btnFd.lockColor = true
                        root.quality = "FD"
                    }
                }
                anchors.fill: parent
            }
        }
        Button {
            id: btnFm
            label: "FM"//"FULLMOVIE"
            labelColor: { if (root.full === "1") "black"; else "white" }
            MouseArea {
                onClicked: {
                    if (root.full === "1") {
                        btnFm.lockColor = false
                        root.full = ""
                    } else {
                        btnFm.lockColor = true
                        root.full = "1"
                    }
                }
                anchors.fill: parent
            }
        }
    }

    Component.onCompleted: {
        if (root.quality === "SD")
            btnSd.lockColor = true
        if (root.quality === "HD")
            btnHd.lockColor = true
        if (root.quality === "FD")
            btnFd.lockColor = true
        if (root.full === "1")
            btnFm.lockColor = true
    }
}
// Tabs hechos.
