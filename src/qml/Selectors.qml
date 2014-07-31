import QtQuick 2.2

Item {
    id: selectors

    Row {
        id: selectorsRow
        spacing: 5
        anchors.left: parent.left
        anchors.leftMargin: 73//196//73
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 32
        z: 2

        Text {
            id: fullMovieLabel
            text: "FULL MOVIE "
            color: { if (root.full == "FULL") "white"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
            MouseArea {
                id: fullMovieMousearea
                onClicked: {
                    if (root.full == "FULL")
                        root.full = ""
                    else
                        root.full= "FULL"
                }
                anchors.fill: parent
            }
        }
        Text {
            id: standardLabel
            text: " SD"
            color: { if (root.quality == "SD") "white"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
            MouseArea {
                id: standardMousearea
                onClicked: {
                    if (root.quality == "SD")
                        root.quality = ""
                    else
                        root.quality = "SD"
                }
                anchors.fill: parent
            }
        }
        Text {
            id: mediumLabel
            text: "720p"
            color: { if (root.quality == "720p") "white"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
            MouseArea {
                id: mediumMousearea
                onClicked: {
                    if (root.quality == "720p")
                        root.quality = ""
                    else
                        root.quality = "720p"
                }
                anchors.fill: parent
            }
        }
        Text {
            id: highLabel
            text: "1080p"
            color: { if (root.quality == "1080p") "white"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 15
            MouseArea {
                id: highMousearea
                onClicked: {
                    if (root.quality == "1080p")
                        root.quality = ""
                    else
                        root.quality = "1080p"
                }
                anchors.fill: parent
            }
        }
    }
}