import QtQuick 2.4

import "../global/"

Item {
    id: quality

    Row {
        spacing: screen.width/192
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        Button {
            id: btnSd
            label: "SD"
            labelColor: { if (root.quality === "SD") "white"; else "gray" }
            labelSize: screen.height/20
            labelColorIn: "white"
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.quality === "SD") root.quality = ""; else root.quality = "SD" }
            }
        }
        Button {
            id: btnHd
            label: "HD"
            labelColor: { if (root.quality === "HD") "white"; else "gray" }
            labelSize: screen.height/20
            labelColorIn: "white"
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.quality === "HD") root.quality = ""; else root.quality = "HD" }
            }
        }
        Button {
            id: btnFd
            label: "FD"
            labelColor: { if (root.quality === "FD") "white"; else "gray" }
            labelSize: screen.height/20
            labelColorIn: "white"
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.quality === "FD") root.quality = ""; else root.quality = "FD" }
            }
        }
        Button {
            id: btnFm
            label: "FM"
            labelColor: { if (root.full === "1") "#CACACA"; else "gray" }
            labelSize: screen.height/20
            labelColorIn: "#CACACA"
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.full === "1") root.full = ""; else root.full = "1" }
            }
        }
    }
}
// Tabs hechos.
