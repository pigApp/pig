import QtQuick 2.2

Item {
    id: errorDbMsg
    Text {
        id: errorStatusLabel
        text: "ERROR IN DATABASE"
        color: "black"
        font.family: pigFont.name
        font.pixelSize: 25
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        id: errorInformationLabel
        text: "statusInformation", "DATABASE DOES NOT EXIST OR IS CORRUPTED" // TODO: Agregar mensaje de donde descargar la db y en que carpeta alojarla.
        color: Qt.rgba(0.5, 0.5, 0.5, 0.3)
        font.family: pigFont.name
        font.pixelSize: 25
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 30
    }
}
