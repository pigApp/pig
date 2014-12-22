import QtQuick 2.3

Rectangle {
    id: filterBox
    height: screen.height/4
    color: Qt.rgba(0, 0, 0, 0.05)
    radius: 2

    property alias labelText: label.text
    property alias totalLabelText: totalLabel.text
    property alias sourceImage: pic.source

    signal clicked()

    ColorAnimation on color { id: inColor; running: false; to: "transparent"; duration: 200 }
    ColorAnimation on color { id: outColor; running: false; to: Qt.rgba(0, 0, 0, 0.05); duration: 200 }

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
        font.bold: true
        font.pixelSize: screen.height/54
        opacity: 0
        anchors.right: parent.right
        anchors.rightMargin: parent.width/96
        anchors.bottom: parent.bottom
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
            PropertyChanges { target: inColor; running: true }
        },
        State {
            name: "out"
            PropertyChanges { target: outColor; running: true }
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
