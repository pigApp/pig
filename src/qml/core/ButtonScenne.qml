import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: buttonScenne
    width: 34
    height: 30
    color: "transparent"

    property alias sceneLabel: label.text
    signal clicked()
    
    Text {
        id: label
        color: { if (label.text != 'SCENE') Qt.rgba(1, 1, 1, 0.15); else Qt.rgba(1, 1, 1, 0.10) }
        font.family: pigFont.name
        font.weight: Font.Light
        font.pixelSize: 11
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: -22
    }

    RectangularGlow {
        id: effect
        color: "white"
        glowRadius: 3
        spread: 0
        cornerRadius: 50
        opacity: 0.2
        visible: false
        anchors.fill: icon
    }
    Image {
        id: icon
        source: { if (label.text != 'SCENE') "qrc:/images/player/play.png"; else "qrc:/images/player/noPlay.png" }
        opacity: { if (label.text != 'SCENE') 0.2; else 0.1 }
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
        onTriggered: { 
            icon.opacity = 1
            effect.visible = true
        }
    }
    Timer {
        id: delayOut
        running: false
        repeat: false
        interval: 20
        onTriggered: { 
            icon.opacity = 0.2
            effect.visible = false
        }
    }
    states: State {
        when: mousearea.pressed && label.text != 'SCENE'
        PropertyChanges { target: icon; opacity: 0.7 }
        PropertyChanges { target: effect; color: Qt.rgba(0.5, 0.5, 0.5, 0.7) }
    }
}
// Espacios hechos.
