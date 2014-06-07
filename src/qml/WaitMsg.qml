import QtQuick 2.2

Item {
  id: waitMsg
    Row {
        id: row
        spacing: 11
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 20
        Text {
            id: msg
            text: "PLEASE WAIT"
            color: Qt.rgba(0.2, 0.2, 0.2, 0.4)
            font.family: pigLightFont.name
            font.pixelSize: 25
        }
        Image {
            id: spinner
            width: 25
            height: 25
            source: "qrc:/images/spinner.png"
            visible: showSpinner
            property bool on: showSpinner
            NumberAnimation { running: spinner.on; target: spinner; properties: "opacity"; from: 0.0; to: 1.0; loops: Animation.Infinite; duration: 2000 }
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 1
        }
    }
}
