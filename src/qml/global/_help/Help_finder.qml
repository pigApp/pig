import QtQuick 2.4

Item {
    id: help_finder

    Canvas {
        width: 200
        height: 200
        onPaint: {
            var ctx = getContext("2d")
            ctx.strokeStyle = "black"
            ctx.beginPath()
            ctx.moveTo(50,50)
            ctx.lineTo(150,50)
            ctx.stroke()
        }
    }
    Text {
        text: "FILTERS"
        color: "black"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.left: parent.left
        anchors.leftMargin: 700
        anchors.verticalCenter: parent.verticalCenter
    }
}
