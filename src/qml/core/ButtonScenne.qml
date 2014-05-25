import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonScenne
    width: 100
    height: 80
    color: "transparent"

    property alias sceneLabel: label.text
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
        source: { if (label.text != 'NOT AVAILABLE') "qrc:/images/1080.png"; else "qrc:/images/notAvailable.png" } // Falta que cargue imagen de 1080p o 720p segun corresponda.
        opacity: { if (label.text != 'NOT AVAILABLE') 1; else 0.15 }
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: { 
            if (label.text != 'NOT AVAILABLE')
                delayIn.restart()
        }
        onHoveredChanged: { 
            if (label.text != 'NOT AVAILABLE')
                delayOut.restart() 
        }
        onClicked: { 
            if (label.text != 'NOT AVAILABLE')
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

    Text {
        id: label
        color: "white"
        font.family: pigFont.name
        font.pixelSize: 15
        opacity: { if (label.text != 'NOT AVAILABLE') 1; else 0.15 }
        anchors.centerIn: parent
        anchors.verticalCenterOffset:  50
    }

    states: State {
        when: mousearea.pressed && label.text != 'NOT AVAILABLE'
        PropertyChanges { target: icon; opacity: 0.9 }
        PropertyChanges { target: effect; color: Qt.rgba(0.5, 0.5, 0.5, 0.7) }
        PropertyChanges { target: label; opacity: 0.9 }
    }
}
// Espacios hechos.
