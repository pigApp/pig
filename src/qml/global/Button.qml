import QtQuick 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: button
    color: "transparent"
   
    
    property alias label: label.text
    property alias labelColor: label.color
    property alias labelBold: label.font.bold
    property alias labelSize: label.font.pixelSize
    property alias labelAngle: rotation.angle
    property alias labelOriginX: rotation.origin.x
    property alias labelOriginY: rotation.origin.y
    property alias labelAxisX: rotation.axis.x
    property alias labelAxisY: rotation.axis.y
    property alias labelAxisZ: rotation.axis.z

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
    
    transform: Rotation {
        id: rotation
        //origin.x: 0
        //origin.y: 0
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
