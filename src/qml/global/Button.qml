import QtQuick 2.4

Rectangle {
    id: button
    color: "transparent"

    property alias label: label.text
    property alias labelColor: label.color
    property alias labelFont: label.font.family
    property alias labelBold: label.font.bold
    property alias labelLetterSpacing: label.font.letterSpacing
    property alias labelSize: label.font.pixelSize
    property alias gridLayerWidth: gridLayer.width
    property alias gridLayerHeight: gridLayer.height
    property alias gridLayerSourceSizeWidth: gridLayer.sourceSize.width
    property alias gridLayerSourceSizeHeight: gridLayer.sourceSize.height
    property alias gridLayerVisible: gridLayer.visible

    signal clicked()

    Image {
        id: gridLayer
        width: 0
        height: 0
        sourceSize.width: 0
        sourceSize.height: 0
        source: "qrc:/img-grid-small"
        visible: false
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: label
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
