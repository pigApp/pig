import QtQuick 2.4

Rectangle {
    id: box
    height: screen.height/4

    property alias color: box.color
    property alias label: label.text
    property alias label_n: label_n.text

    property string source: ""

    Image {
        id: pic
        opacity: 0
        anchors.fill: parent
    }

    Text {
        id: label
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.centerIn: parent
    }
    Text {
        id: label_n
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        opacity: 0
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: screen.width/960
    }

    MouseArea {
        hoverEnabled: true
        enabled: { label.text !== "" }
        anchors.fill: parent
        onEntered: {
            pic.source = source
            box.state = "in"
        }
        onHoveredChanged: { box.state = "out" }
        onClicked: { if (label_n.text !== "000") filters.set_filter(label.text) }
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
                NumberAnimation { target: pic; properties: "opacity"
                    ; to: 1; duration: 1000; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label; properties: "opacity"
                    ; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label_n; properties: "opacity"
                    ; to: 1; duration: 800; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "out"
            ParallelAnimation {
                NumberAnimation { target: pic; properties: "opacity"
                    ; to: 0; duration: 100; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label; properties: "opacity"
                    ; to: 1; duration: 200; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label_n; properties: "opacity"
                    ; to: 0; duration: 100; easing.type: Easing.InOutQuart }
            }
        }
    ]
}
// Tabs hechos.
