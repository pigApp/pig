import QtQuick 2.4

Item {
    id: help_viewer

    Rectangle {
        id: layerUp
        width: labelUp.contentWidth
        height: labelUp.contentHeight
        color: Qt.rgba(0, 0.28, 0.047, 1)
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -(layerUp.height+labelUpArrow.contentHeight)
    }
    Text {
        id: labelUp
        text: " MORE "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerUp
    }
    Text {
        id: labelUpArrow
        text: "↑"
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.top: layerUp.bottom
        anchors.topMargin: -parent.height/108
        anchors.horizontalCenter: layerUp.horizontalCenter
    }
    Rectangle {
        id: layerLeft
        width: labelLeft.contentWidth
        height: labelLeft.contentHeight
        color: Qt.rgba(0, 0.28, 0.047, 1)
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -(layerLeft.width-(parent.width/192))
        anchors.verticalCenterOffset: -parent.height/43.2
    }
    Text {
        id: labelLeft
        text: " ROTATE "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerLeft
    }
    Text {
        id: labelLeftArrow
        text: "←"
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.left: layerLeft.right
        anchors.leftMargin: parent.width/192
        anchors.verticalCenter: layerLeft.verticalCenter
    }
    Rectangle {
        id: layerRight
        width: layerLeft.width
        height: layerLeft.height
        color: Qt.rgba(0, 0.28, 0.047, 1)
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: layerRight.width-(parent.width/192)
        anchors.verticalCenterOffset: -parent.height/43.2
    }
    Text {
        id: labelRight
        text: " ROTATE "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerRight
    }
    Text {
        id: labelRightArrow
        text: "→"
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.right: layerRight.left
        anchors.rightMargin: parent.width/192
        anchors.verticalCenter: layerRight.verticalCenter
    }
    Rectangle {
        id: layerDown
        width: layerUp.width
        height: labelUp.height
        color: Qt.rgba(0, 0.28, 0.047, 1)
        anchors.centerIn: parent
        anchors.verticalCenterOffset: (layerDown.height+labelDownArrow.contentHeight)-(parent.height/27)
    }
    Text {
        id: labelDown
        text: "LESS "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerDown
    }
    Text {
        id: labelDownArrow
        text: "↓"
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.horizontalCenter: layerDown.horizontalCenter
        anchors.bottom: layerDown.top
    }

    Rectangle {
        id: layerCover
        width: labelCover.contentWidth
        height: labelCover.contentHeight
        color: "black"
        anchors.left: parent.left
        anchors.leftMargin: parent.width/2.77
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/4.55
    }
    Text {
        id: labelCover
        text: " FRONT / BACK "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerCover
    }

    Rectangle {
        id: layerPreview
        width: labelPreview.contentWidth
        height: labelPreview.contentHeight
        color: "black"
        anchors.right: parent.right
        anchors.rightMargin: parent.width/5.8
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/43.2
    }
    Text {
        id: labelPreview
        text: " PREVIEW "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerPreview
    }

    Rectangle {
        id: layerBack
        width: labelBack.contentWidth
        height: labelBack.contentHeight
        color: "black"
        anchors.top: parent.top
        anchors.topMargin: parent.height/21.6
        anchors.left: parent.left
        anchors.leftMargin: parent.width/38.4
    }
    Rectangle {
        id: layerBackKey
        width: layerBack.width
        height: layerBack.height
        color: "white"
        anchors.top: layerBack.bottom
        anchors.horizontalCenter: layerBack.horizontalCenter
    }
    Text {
        id: labelBack
        text: " BACK "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerBack
    }
    Text {
        id: labelBackKey
        text: "ESC"
        color: "black"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerBackKey
    }

    Rectangle {
        id: layerQuit
        width: layerQuitKey.width
        height: layerQuitKey.height
        color: "red"
        anchors.top: parent.top
        anchors.topMargin: parent.height/21.6
        anchors.right: parent.right
        anchors.rightMargin: parent.width/38.4
    }
    Rectangle {
        id: layerQuitKey
        width: labelQuitKey.contentWidth
        height: labelQuitKey.contentHeight
        color: "white"
        anchors.top: layerQuit.bottom
        anchors.horizontalCenter: layerQuit.horizontalCenter
    }
    Text {
        id: labelQuit
        text: "QUIT"
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerQuit
    }
    Text {
        id: labelQuitKey
        text: " CTRL Q "
        color: "black"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerQuitKey
    }
}
// Tabs hechos.
