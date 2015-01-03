import QtQuick 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: button
    color: "transparent"
   
    property alias labelFont: label.font.family
    property alias label: label.text
    property alias labelColor: label.color
    property alias labelBold: label.font.bold
    property alias labelSize: label.font.pixelSize
    property alias layerWidth: layer.width
    property alias layerHeight: layer.height
    property alias layerColor: layer.color
    property alias layerVisible: layer.visible

    signal clicked()

    Rectangle {
        id: layer
        width: 0
        height: 0
        visible: false
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        id: label
        anchors.verticalCenter: parent.verticalCenter
    }
    DropShadow {
        id: shadow
        color: "white"
        source: label
        radius: 8
        samples: 32
        opacity: 0
        anchors.fill: label
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
            NumberAnimation { target: shadow; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "out"
            NumberAnimation { target: shadow; easing.amplitude: 1.7; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.OutQuart }
        }
    ]
}
// Tabs hechos.
