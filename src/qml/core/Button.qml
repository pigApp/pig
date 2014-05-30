import QtQuick 2.2

Rectangle {
    id: button
    color: "transparent"

    property alias label: label.text
    property alias labelColor: label.color

    signal clicked()
    
    Text {
        id: label
        color: "white"
        font.family: pigFont.name
        font.pixelSize: 110
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
    }

    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: delayIn.restart()
        onHoveredChanged: delayOut.restart()
        onClicked: button.clicked()
        anchors.fill: parent
    }
    Timer {
        id: delayIn
        running: false
        repeat: false
        interval: 25
        onTriggered: { label.color = Qt.rgba(0, 0, 0, 0.4) }
    }
    Timer {
        id: delayOut
        running: false
        repeat: false
        interval: 20
        onTriggered: { label.color = "white" }
    }
    states: State {
        when: mousearea.pressed
        PropertyChanges { target: label; color: Qt.rgba(0, 0, 0, 0.2) }
    }
}
// Espacios hechos.
