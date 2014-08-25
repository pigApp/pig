import QtQuick 2.2

Item {
    id: wait

    Image {
        id: network
        width: parent.width/5.36
        height: parent.height/3.01
        source: "qrc:/images/pig/network.png"
        sourceSize.width: 358
        sourceSize.height: 358
        anchors.centerIn: parent
        SequentialAnimation {
            running: true
            loops: Animation.Infinite
            PropertyAction { target: witnessStatusLabel; property: "visible"; value: "true" }
            PauseAnimation { duration: 500 }
            PropertyAction { target: witnessStatusLabel; property: "visible"; value: "false" }
            PauseAnimation { duration: 500 }
        }
    }
    Text {
        id: witnessStatusLabel
        text: "•"
        color: "white"
        font.family: pigFont.name
        font.pixelSize: parent.height/43.2
        visible: { !root.showNetwork }
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: parent.width/3500
        anchors.verticalCenterOffset: parent.height/130
    }
}
