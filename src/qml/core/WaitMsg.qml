import QtQuick 2.2

Item {
  id: waitMsg
    Row {
        id: row
        spacing: 15
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -27
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: msg
            text: "PLEASE WAIT"
            color: Qt.rgba(0.2, 0.2, 0.2, 0.4)
            font.family: pigLightFont.name
            font.pixelSize: 25
        }
        Image {
            id: spinner
            width: 29.3
            height: 29.3
            source: "qrc:/images/spinner.png";
            NumberAnimation on rotation { running: true; from: 0; to: 360; loops: Animation.Infinite; duration: 1200 }
        }
    }
}
