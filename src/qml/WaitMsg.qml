import QtQuick 2.2

Item {
  id: waitMsg
    Row {
        id: row
        spacing: 17
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 20
        Text {
            id: msg
            text: "PLEASE WAIT"
            color: Qt.rgba(0.1, 0.1, 0.1, 0.13)
            font.family: pigFont.name
            font.pixelSize: 30
        }
        Image {
            id: spinner
            width: 25
            height: 25
            source: "qrc:/images/spinner.png"
            fillMode: Image.PreserveAspectCrop
            clip: true
            visible: showSpinner
            property bool on: showSpinner
            NumberAnimation on rotation { running: spinner.on; from: 0; to: 360; loops: Animation.Infinite; duration: 1200 }
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
