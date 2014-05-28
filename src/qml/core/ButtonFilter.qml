import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonFilter
    height: 200
    color: Qt.rgba(0 ,0 ,0 , 0.03)
    radius: 2 

    property alias labelText: label.text
    property alias nLabelText: nLabel.text
    property alias sourceImage: starPic.source
    property bool pornStarsVisible

    signal clicked()
    
    RectangularGlow {
        id: effect
        color: Qt.rgba(1, 1, 1, 0.4)
        glowRadius: 15
        spread: 0
        cornerRadius: 20
        visible: false
        anchors.fill: buttonFilter
    }
    Image {
        id: starPic
        visible: false
        anchors.centerIn: parent
    }
    Text {
        id: label
        color: "white"
        font.family: pigFont.name
        font.capitalization: Font.AllUppercase
        font.pixelSize: 40
        anchors.centerIn: parent
    }
    Text {
        id: nLabel
        color: Qt.rgba(1, 1, 1, 0.15)
        font.family: pigFont.name
        font.pixelSize: 30
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 50
    }

    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: delayIn.restart()
        onHoveredChanged: delayOut.restart()
        onClicked: buttonFilter.clicked()
        anchors.fill: parent
    }
    Timer {
        id: delayIn
        running: false
        repeat: false
        interval: 25
        onTriggered: { 
            if (pornStarsVisible) {
                starPic.visible = true
                label.visible = false
                nLabel.visible = false
                buttonFilter.color = "white"
            } else {
                label.color = Qt.rgba(0, 0, 0, 1)
                buttonFilter.color = "white"
            }
            effect.visible = true
        }
    }
    Timer {
        id: delayOut
        running: false
        repeat: false
        interval: 18
        onTriggered: { 
            if (pornStarsVisible) {
                starPic.visible = false
                label.visible = true
                nLabel.visible = true
                buttonFilter.color = Qt.rgba(0 ,0 ,0 , 0.03)
            } else {
                label.color = "white"
                buttonFilter.color = Qt.rgba(0 ,0 ,0 , 0.03)
            }
            effect.visible = false
        }
    }
    states: State {
        when: mousearea.pressed && !pornStarsVisible
        PropertyChanges { target: buttonFilter; color: Qt.rgba(1, 1, 1, 0.8) }
    }
}
// Espacios hechos.
