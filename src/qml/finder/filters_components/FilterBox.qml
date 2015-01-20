import QtQuick 2.4

Item {
    id: filterBox
    height: screen.height/4

    property alias labelText: label.text
    property alias totalLabelText: totalLabel.text
    property alias sourceImage: pic.source

    signal clicked()

    Image {
        id: gridLayer
        sourceSize.width: 480
        sourceSize.height: 270
        source: "qrc:/img-grid-filters"
        anchors.fill: parent
    }

    Image {
        id: pic
        enabled: { pic.source !== "" }
        opacity: 0
        anchors.fill: parent
    }
    Text {
        id: label
        color: "white"
        font.family: globalFont.name
        font.pixelSize: screen.height/23
        anchors.centerIn: parent
    }
    Text {
        id: totalLabel
        color: "white"
        font.family: globalFont.name
        font.pixelSize: screen.height/23
        opacity: 0
        anchors.right: parent.right
        anchors.rightMargin: screen.width/192
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -screen.height/216
    }

    MouseArea {
        hoverEnabled: true
        onEntered: { filterBox.state = "in" }
        onHoveredChanged: { filterBox.state = "out" }
        onClicked: filterBox.clicked()
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
            ParallelAnimation {
                NumberAnimation { target: pic; properties: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                NumberAnimation { target: totalLabel; properties: "opacity"; to: 1; duration: 800; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "out"
            ParallelAnimation {
                NumberAnimation { target: pic; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
                NumberAnimation { target: totalLabel; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
            }
        }
    ]
}
// Tabs hechos.
