import QtQuick 2.4

Item {
    id: help_finder

    Rectangle {
        id: layerSearch
        width: labelSearch.contentWidth
        height: labelSearch.contentHeight
        color: "black"
        anchors.left: parent.left
        anchors.leftMargin: parent.width/11.42
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/10.8
    }
    Rectangle {
        id: layerSearchKey
        width: layerSearch.width
        height: layerSearch.height
        color: "white"
        anchors.top: layerSearch.bottom
        anchors.horizontalCenter: layerSearch.horizontalCenter
    }
    Text {
        id: labelSearch
        text: " SEARCH "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerSearch
    }
    Text {
        id: labelSearchKey
        text: "INTRO"
        color: "black"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerSearchKey
    }

    Rectangle {
        id: layerFilters
        width: labelFilters.contentWidth
        height: labelFilters.contentHeight
        color: "black"
        anchors.left: parent.left
        anchors.leftMargin: parent.width/2.83
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/10.8
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
        id: layerSelectors
        width: labelSelectors.contentWidth
        height: labelSelectors.contentHeight
        color: "black"
        anchors.left: parent.left
        anchors.leftMargin: parent.width/1.84
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/10.8
    }
    Text {
        id: labelSelectors
        text: " SELECTORS "
        color: "white"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/23
        anchors.centerIn: layerSelectors
    }

    Rectangle {
        id: layerSetting
        width: labelSetting.contentWidth
        height: labelSetting.contentHeight
        color: "black"
        anchors.right: parent.right
        anchors.rightMargin: parent.width/38.4
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/10.8
    }
    Rectangle {
        id: layerSettingKey
        width: layerSetting.width
        height: layerSetting.height
        color: "white"
        anchors.top: layerSetting.bottom
        anchors.horizontalCenter: layerSetting.horizontalCenter
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
    Text {
        id: labelSettingKey
        text: "CTRL S"
        color: "black"
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
