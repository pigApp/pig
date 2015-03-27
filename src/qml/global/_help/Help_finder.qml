import QtQuick 2.4

Item {
    id: help_finder

    /*
    Canvas {
        width: 1920//
        height: 1080//
        onPaint: {
            var ctx = getContext("2d")
            ctx.lineWidth = 1
            ctx.strokeStyle = "white"
            ctx.beginPath()

            ctx.moveTo(255, 585)//
            ctx.lineTo(255, 685)//

            ctx.moveTo(760, 585)//
            ctx.lineTo(760, 685)//

            ctx.moveTo(1124, 585)//
            ctx.lineTo(1124, 685)//

            ctx.moveTo(1276, 585)//
            ctx.lineTo(1276, 685)//

            ctx.moveTo(1750, 20)//
            ctx.lineTo(1750, 70)//

            ctx.stroke()
        }
    }
    */

    Rectangle {
        id: layerSearch
        width: labelSearch.contentWidth
        height: labelSearch.contentHeight
        color: "gray"//"white"
        anchors.left: parent.left
        anchors.leftMargin: 176//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 100//
    }
    Text {
        id: labelSearch
        text: " SEARCH "
        color: "white"//"black"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerSearch
    }
    Rectangle {
        id: layerSearchKey
        width: labelSearchKey.contentWidth
        height: labelSearchKey.contentHeight
        color: "black"
        anchors.top: labelSearch.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: labelSearch.horizontalCenter
    }
    Text {
        id: labelSearchKey
        text: " INTRO "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerSearchKey
    }

    Rectangle {
        id: layerFilters
        width: labelFilters.contentWidth
        height: labelFilters.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 685//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 100//
    }
    Text {
        id: labelFilters
        text: " FILTERS "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerFilters
    }

    Rectangle {
        id: layerQuality
        width: labelQuality.contentWidth
        height: labelQuality.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 1040//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 100//
    }
    Text {
        id: labelQuality
        text: " QUALITY "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerQuality
    }

    Rectangle {
        id: layerFull
        width: labelFull.contentWidth
        height: labelFull.contentHeight
        color: "gray"
        anchors.left: parent.left
        anchors.leftMargin: 1227//
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 100//
    }
    Text {
        id: labelFull
        text: " FULL "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerFull
    }

    Rectangle {
        id: layerSetting
        width: labelSetting.contentWidth
        height: labelSetting.contentHeight
        color: "gray"
        anchors.left: parent.left
        //anchors.leftMargin: 43//
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
    }
    Text {
        id: labelSetting
        text: " SETTING "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerSetting
    }
    Rectangle {
        id: layerSettingKey
        width: labelSettingKey.contentWidth
        height: labelSettingKey.contentHeight
        color: "black"
        anchors.top: labelSetting.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: labelSetting.horizontalCenter
    }
    Text {
        id: labelSettingKey
        text: " CTRL S "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerSettingKey
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
        color: "gray"
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
        font.pixelSize: screen.height/23
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
        font.pixelSize: screen.height/23
        anchors.centerIn: layerQuitKey
    }
}
