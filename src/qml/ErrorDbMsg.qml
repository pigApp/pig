import QtQuick 2.2

Item {
    id: errorDbMsg

    Rectangle {
        id: layer
        width: 700
        height: 200
        color: "black"
        anchors.centerIn: parent

        Text {
            id: errorStatusLabel
            text: "ERROR IN DATABASE"
            color: "white"
            font.family: pigFont.name
            font.pixelSize: 30
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -20
        }
        Text {
            id: errorInformationLabel
            text: "DATABASE NOT EXIST OR IS CORRUPTED"
            color: "white"
            font.family: pigFont.name
            font.pixelSize: 30
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 10
        }
        Text {
            id: getDbInformationLabel
            text: "GET LATEST DATABASE FROM https://sourceforge/pigApp/db" // TODO: Url real db.
            color: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 40
        }
    }
}
