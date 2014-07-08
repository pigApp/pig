import QtQuick 2.2
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonFilter
    height: 200
    color: Qt.rgba(0 ,0 ,0 , 0.03)
    radius: 2

    ColorAnimation on color { to: Qt.rgba(1, 0, 0, 0.5); duration: 400 }
    ColorAnimation on color { id: inColor; running: false; to: "white"; duration: 200 }
    ColorAnimation on color { id: outColor; running: false; to: Qt.rgba(1, 0, 0, 0.5); duration: 200 }

    property bool enter
    property bool pornStarsVisible
    property alias labelText: label.text
    property alias nLabelText: nLabel.text
    property alias sourceImage: starPic.source

    signal clicked()
    
    RectangularGlow {
        id: effect
        color: Qt.rgba(1, 1, 1, 0.4)
        glowRadius: 15
        spread: 0
        cornerRadius: 20
        opacity: 0
        anchors.fill: buttonFilter
    }
    Image {
        id: starPic
        opacity: 0
        anchors.centerIn: parent
    }

    Text {
        id: nLabel
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: 150
        visible: false
        opacity: 0
        anchors.centerIn: parent
    }
    Text {
        id: label
        color: "white"
        font.family: pigFont.name
        font.capitalization: Font.AllUppercase
        font.pixelSize: 50
        visible: false
        anchors.centerIn: parent
        ColorAnimation on color { id: inLabelColor; running: false; to: "black"; duration: 200 }
        ColorAnimation on color { id: outLabelColor; running: false; to: "white"; duration: 200 }
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: { if (!labelDelay.running) enter = true }
        onHoveredChanged: { if (!labelDelay.running) enter = false }
        onClicked: { buttonFilter.clicked() }
        anchors.fill: parent
    }
    onEnterChanged: {
        if (enter) {
            if (pornStarsVisible) {
                inStarPic.start()
                inLabel.start()
                inNLabel.start()
                inColor.start()
                inEffect.start()
            } else {
                inLabelColor.start()
                inNLabel.start()
                inColor.start()
            }
        } else {
            if (pornStarsVisible) {
                inStarPic.stop()
                inLabel.stop()
                inNLabel.stop()
                inColor.stop()
                inEffect.stop()
                outStarPic.start()
                outLabel.start()
                outNLabel.start()
                outColor.start()
                outEffect.start()
            } else {
                inLabelColor.stop()
                inNLabel.start()
                inColor.stop()
                outLabelColor.start()
                outNLabel.start()
                outColor.start()
            }
        }
    }
    Timer {
        id: labelDelay
        running: false
        repeat: false
        interval: 450
        onTriggered: {
            label.visible = true
            if (pornStarsVisible)
                nLabel.visible = false
            else
                nLabel.visible = true
        }
    }
    NumberAnimation { id: inStarPic; target: starPic; properties: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuart }
    NumberAnimation { id: inLabel; target: label; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: inNLabel; target: nLabel; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: inEffect; target: effect; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outStarPic; target: starPic; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outLabel; target: label; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outNLabel; target: nLabel; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outEffect; target: effect; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }

    Component.onCompleted: { labelDelay.start(); buttonFilter.forceActiveFocus() }
}
// Espacios hechos.
