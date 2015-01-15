import QtQuick 2.4

Rectangle {
    id: button
    color: "transparent"

    property alias label: label.text
    property alias labelColor: label.color
    property alias labelLeftMargin: button.leftMargin
    property alias gridLayerVisible: gridLayer.visible

    property int leftMargin: 0

    signal clicked()

    Image {
        id: gridLayer
        width: parent.width
        height: parent.height
        sourceSize.width: 455
        sourceSize.height: 50
        source: { if (gridLayer.visible) "qrc:/img-grid-small"; else "" }
        visible: false
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: label
        color: "white"
        font.family: globalFont.name
        font.pixelSize: screen.height/23
        anchors.left: parent.left
        anchors.leftMargin: leftMargin
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        hoverEnabled: true
        onEntered: { button.state = "in" }
        onHoveredChanged: { button.state = "out" }
        onClicked: button.clicked()
        anchors.fill: parent
    }
    
    states: [
        State {
            name: "in"
        },
        State {
            name: "out"
        }
    ]
    transitions: [
        Transition {
            to: "in"
            NumberAnimation { target: label; easing.amplitude: 1.7; properties: "opacity"; to: 0.7; duration: 100; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "out"
            NumberAnimation { target: label; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 100; easing.type: Easing.OutQuart }
        }
    ]
}
// Tabs hechos.
