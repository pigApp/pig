import QtQuick 2.2

Item {
    id: bar
    width: 250
    height: 10
    clip: true

    property int minimum: 0
    property int maximum: 100
    property int value
    onValueChanged: { bar.maximum = root.neededPieces }

    Rectangle {
        id: border
        width: 250
        height: 2
        color: "transparent"
        border.width: 1
        border.color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 2
    }
    Rectangle {
        id: highlight
        height: 4
        color: "white"
        property int widthDest: ((bar.width * (value-minimum))/(maximum-minimum)-4)
        width: highlight.widthDest
        Behavior on width { SmoothedAnimation { velocity: 1200 } }
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 2
    }
}
