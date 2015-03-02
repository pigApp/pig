import QtQuick 2.4

import "../global/"

Item {
    id: quality

    Row {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        Button {
            id: btnSd
            width: screen.width/25.26
            height: screen.height/14.21
            iconSource: { if (root.quality === "SD") "qrc:/img-sd"; else "qrc:/img-sd-off" }
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.quality === "SD") root.quality = ""; else root.quality = "SD" }
            }
        }
        Button {
            id: btnHd
            width: screen.width/25.26
            height: screen.height/14.21
            iconSource: { if (root.quality === "HD") "qrc:/img-hd"; else "qrc:/img-hd-off" }
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.quality === "HD") root.quality = ""; else root.quality = "HD" }
            }
        }
        Button {
            id: btnFhd
            width: screen.width/25.26
            height: screen.height/14.21
            iconSource: { if (root.quality === "FD") "qrc:/img-fhd"; else "qrc:/img-fhd-off" }
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.quality === "FD") root.quality = ""; else root.quality = "FD" }
            }
        }
        Button {
            id: btnFm
            width: screen.width/25.26
            height: screen.height/14.21
            iconSource: { if (root.full === "1") "qrc:/img-fm"; else "qrc:/img-fm-off" }
            MouseArea {
                anchors.fill: parent
                onClicked: { if (root.full === "1") root.full = ""; else root.full = "1" }
            }
        }
    }
}
// Tabs hechos.
