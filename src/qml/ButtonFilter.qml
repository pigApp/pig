import QtQuick 2.2
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonFilter
    height: screen.height/4
    color: "black"
    radius: 2

    ColorAnimation on color { to: "black" }
    ColorAnimation on color { id: inColor; running: false; to: "white"; duration: 200 }
    ColorAnimation on color { id: outColor; running: false; to: "black"; duration: 200 }

    property bool enter

    property alias labelText: label.text
    property alias indicatorLabelvisible: indicatorLabel.visible
    property alias numberLabelText: numberLabel.text
    property alias sourceImage: pic.source

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
        id: pic
        width: screen.width/4
        height: screen.height/4
        sourceSize.width: screen.width/4
        sourceSize.height: screen.height/4
        opacity: 0
        anchors.centerIn: parent
    }
    Text {
        id: label
        color: "white"
        font.family: pigFont.name
        font.pixelSize: screen.height/23
        anchors.centerIn: parent
    }
    Text {
        id: indicatorLabel
        text: "∙"
        color: "red"
        font.family: pigFont.name
        font.pixelSize: screen.height/21.5
        visible: false
        anchors.left: label.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -screen.height/3600
    }
    Text {
        id: numberLabel
        color: "white"
        font.family: pigFont.name
        font.pixelSize: screen.height/21.5
        opacity: 0
        anchors.right: parent.right
        anchors.rightMargin: parent.width/48
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -parent.height/135
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: { enter = true }
        onHoveredChanged: { enter = false }
        onClicked: { buttonFilter.clicked() }
        anchors.fill: parent
    }
    onEnterChanged: {
        if (enter) {
            inStarPic.start()
            inLabel.start()
            outIndicatorLabel.stop()
            inIndicatorLabel.start()
            inNumberLabel.start()
            inColor.start()
            inEffect.start()
        } else {
            inStarPic.stop()
            inLabel.stop()
            inIndicatorLabel.stop()
            inNumberLabel.stop()
            inColor.stop()
            inEffect.stop()

            outStarPic.start()
            outLabel.start()
            outIndicatorLabel.start()
            outNumberLabel.start()
            outColor.start()
            outEffect.start()
        }
    }
    NumberAnimation { id: inStarPic; target: pic; properties: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuart }
    NumberAnimation { id: inLabel; target: label; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: inIndicatorLabel; target: indicatorLabel; properties: "opacity"; to: 0; duration: 1; easing.type: Easing.InOutQuart }
    NumberAnimation { id: inNumberLabel; target: numberLabel; properties: "opacity"; to: 1; duration: 1100; easing.type: Easing.InOutQuart }
    NumberAnimation { id: inEffect; target: effect; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outStarPic; target: pic; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outLabel; target: label; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outIndicatorLabel; target: indicatorLabel; properties: "opacity"; to: 1; duration: 500; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outNumberLabel; target: numberLabel; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
    NumberAnimation { id: outEffect; target: effect; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }

    Component.onCompleted: { buttonFilter.forceActiveFocus() }
}
// Espacios hechos.
