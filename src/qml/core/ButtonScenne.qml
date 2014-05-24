import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonScenne
    width: 34
    height: 30
    color: "transparent"

    property alias sceneLabel: label.text
    signal clicked()
    
    RectangularGlow {
        id: effect
        color: "white"
        glowRadius: 5
        spread: 0
        cornerRadius: 50
        opacity: 0.2
        visible: false
        anchors.fill: icon
    }
    Image {
        id: icon
        source: { if (label.text != 'SCENE') "qrc:/images/player/play.png"; else "qrc:/images/player/noPlay.png" }
        opacity: { if (label.text != 'SCENE') 1; else 0.15 }
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
    }
    MouseArea {
        id: mousearea
        hoverEnabled: true
        onEntered: { 
            if (label.text != 'SCENE')
                delayIn.restart()
        }
        onHoveredChanged: { 
            if (label.text != 'SCENE')
                delayOut.restart() 
        }
        onClicked: { 
            if (label.text != 'SCENE')
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
        font.pixelSize: 11
        opacity: { if (label.text != 'SCENE') 1; else 0.15 }
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: 2
        anchors.verticalCenterOffset:  27
    }

    states: State {
        when: mousearea.pressed && label.text != 'SCENE'
        PropertyChanges { target: icon; opacity: 0.15 }
        PropertyChanges { target: effect; color: Qt.rgba(0.5, 0.5, 0.5, 0.7) }
        PropertyChanges { target: label; opacity: 0.15 }
    }
}
// Espacios hechos.
