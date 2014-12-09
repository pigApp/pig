import QtQuick 2.3

Item {
    id: progressBar
    width: parent.width

    property int minimum: 0
    property int maximum: 100
    property int value

    Rectangle {
        id: border
        width: progressBar.width
        height: 2
        color: "transparent"
        border.width: 1
        border.color: Qt.rgba(0.1, 0.1, 0.1, 1)
    }
    Rectangle {
        id: highlight
        height: 2
        color: "white"
        property int widthDest: ((progressBar.width*(value-minimum))/(maximum-minimum)-4)
        width: highlight.widthDest
        Behavior on width { SmoothedAnimation { velocity: 1200 } }
    }

    onValueChanged: { progressBar.maximum = root.required }
}
