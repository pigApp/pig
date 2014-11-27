import QtQuick 2.3

Rectangle {
    id: buttonFilter
    height: screen.height/4
    color: Qt.rgba(0, 0, 0, 0.05)
    radius: 2

    property alias labelText: label.text
    property alias numberLabelText: numberLabel.text
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
        font.family: pigFont.name
        font.pixelSize: screen.height/23
        anchors.centerIn: parent
    }
    Image {
        id: numberIcon
        width: parent.width/7.5
        height: parent.height/4.21
        sourceSize.width: parent.width/7.5
        sourceSize.height: parent.height/4.21
        source: "/resources/images/finder/filters/icons/number.svg"
        opacity: 0
        anchors.right: parent.right
        anchors.rightMargin: parent.width/32
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        id: numberLabel
        color: "white"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        opacity: 0
        anchors.centerIn: numberIcon
    }
    MouseArea {
        hoverEnabled: true
        onEntered: { buttonFilter.state = "in" }
        onHoveredChanged: { buttonFilter.state = "out"; buttonFilter.forceActiveFocus() }
        onClicked: buttonFilter.clicked()
        anchors.fill: parent
    }

    states: [
        State {
            name: "in"
            PropertyChanges {target: inColor; running: true}
        },
        State {
            name: "out"
            PropertyChanges {target: outColor; running: true}
        }
    ]
    transitions: [
        Transition {
            to: "in"
            ParallelAnimation {
                NumberAnimation { target: pic; properties: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                NumberAnimation { target: numberIcon; properties: "opacity"; to: 0.5; duration: 800; easing.type: Easing.InOutQuart }
                NumberAnimation { target: numberLabel; properties: "opacity"; to: 1; duration: 800; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "out"
            ParallelAnimation {
                NumberAnimation { target: pic; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
                NumberAnimation { target: label; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
                NumberAnimation { target: numberIcon; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
                NumberAnimation { target: numberLabel; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
            }
        }
    ]

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            finder.state = "hideFilter"
            event.accepted = true
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true
        }
    }

    Component.onCompleted: buttonFilter.forceActiveFocus()
}
// Tabs hechos.
