import QtQuick 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: button
    color: "transparent"
   
    transform: Rotation { origin.x: 25; origin.y: 25; angle: -25}
    
    property alias label: label.text
    property alias labelColor: label.color
    property alias labelBold: label.font.bold
    property alias labelSize: label.font.pixelSize

    signal clicked()

    Text {
        id: label
        font.family: globalFont.name
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
