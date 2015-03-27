import QtQuick 2.4

Item {
    id: help_viewer

    /*
    Canvas {
        width: 1920//
        height: 1080//
        onPaint: {
            var ctx = getContext("2d")
            ctx.lineWidth = 2
            ctx.strokeStyle = "white"
            ctx.beginPath()

            ctx.moveTo(820, 855)//
            ctx.lineTo(820, 955)//

            ctx.moveTo(1500, 795)//
            ctx.lineTo(1500, 895)//

            ctx.moveTo(90, 20)//
            ctx.lineTo(90, 70)//

            ctx.moveTo(1750, 20)//
            ctx.lineTo(1750, 70)//

            ctx.stroke()
        }
    }
    */

    Rectangle {
        id: layerUp
        width: labelUp.contentWidth
        height: labelUp.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 220//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 300
    }
    Text {
        id: labelUp
        text: " 5 MORE "
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
        //font.bold: true
        font.pixelSize: screen.height/10
        anchors.top: layerUp.bottom
        anchors.topMargin: -40
        anchors.horizontalCenter: layerUp.horizontalCenter
        anchors.horizontalCenterOffset: -10
    }

    Rectangle {
        id: layerLeft
        width: labelLeft.contentWidth
        height: labelLeft.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 50//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 400
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
        //font.bold: true
        font.pixelSize: screen.height/10
        anchors.left: layerLeft.right
        anchors.leftMargin: 5
        anchors.verticalCenter: layerLeft.verticalCenter
        anchors.verticalCenterOffset: -10
    }

    Rectangle {
        id: layerRight
        width: labelRight.contentWidth
        height: labelRight.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 365//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 400
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
        //font.bold: true
        font.pixelSize: screen.height/10
        anchors.right: layerRight.left
        anchors.rightMargin: 5
        anchors.verticalCenter: layerRight.verticalCenter
        anchors.verticalCenterOffset: -10
    }

    Rectangle {
        id: layerDown
        width: labelDown.contentWidth
        height: labelDown.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 215//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 500
    }
    Text {
        id: labelDown
        text: " 5 LESS  "
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
        //font.bold: true
        font.pixelSize: screen.height/10
        anchors.bottom: layerDown.top
        anchors.bottomMargin: -20
        anchors.horizontalCenter: layerDown.horizontalCenter
        //anchors.horizontalCenterOffset: -10
    }

    Rectangle {
        id: layerCover
        width: labelCover.contentWidth
        height: labelCover.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 630//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 455
    }
    Text {
        id: labelCover
        text: " FRONT · BACK "
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
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 1410//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 400
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
        color: "gray"
        anchors.top: parent.top
        anchors.topMargin: 50//
        anchors.left: parent.left
        anchors.leftMargin: 43//
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
    Rectangle {
        id: layerBackKey
        width: labelBackKey.contentWidth
        height: labelQuitKey.contentHeight
        color: "black"
        anchors.top: labelBack.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: labelBack.horizontalCenter
    }
    Text {
        id: labelBackKey
        text: " ESC "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/30
        anchors.centerIn: layerBackKey
    }

    Rectangle {
        id: layerQuit
        width: labelQuit.contentWidth
        height: labelQuit.contentHeight
        color: "red"
        anchors.top: parent.top
        anchors.topMargin: 50//
        anchors.left: parent.left
        anchors.leftMargin: 1703//
    }
    Text {
        id: labelQuit
        text: " QUIT "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerQuit
    }
    Rectangle {
        id: layerQuitKey
        width: labelQuitKey.contentWidth
        height: labelQuitKey.contentHeight
        color: "black"
        anchors.top: labelQuit.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: labelQuit.horizontalCenter
    }
    Text {
        id: labelQuitKey
        text: " CTRL Q "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/30
        anchors.centerIn: layerQuitKey
    }
}
