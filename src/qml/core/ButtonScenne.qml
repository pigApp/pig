import QtQuick 2.2
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonScenne
    width: 100
    height: 80
    color: "transparent"

    property alias iconOpacity: icon.opacity
    property alias iconQuality: icon.source

    signal clicked()
    
    RectangularGlow {
        id: effect
        color: "white"
        glowRadius: 5
        spread: 0
        cornerRadius: 50
        opacity: 0.6
        visible: false
        anchors.fill: icon
    }
    Image {
        id: icon
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: { 
            if (icon.opacity != 0.15)
                delayIn.restart()
        }
        onHoveredChanged: { 
            if (icon.opacity != 0.15)
                delayOut.restart() 
        }
        onClicked: { 
            if (icon.opacity != 0.15) 
                buttonScenne.clicked()
        }
        anchors.fill: parent
    }
    Timer {
        id: delayIn
        running: false
        repeat: false
        interval: 25
        onTriggered: { effect.visible = true }
    }
    Timer {
        id: delayOut
        running: false
        repeat: false
        interval: 20
        onTriggered: { effect.visible = false }
    }
}
// Espacios hechos.
