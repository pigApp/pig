import QtQuick 2.2

Item {
    id: update

    Image {
        id: networkIcon
        width: parent.width/5.36
        height: parent.height/3.01
        source: "qrc:/images/pig/network.png"
        sourceSize.width: 358
        sourceSize.height: 358
        visible: { root.showNetworkIcon }
        anchors.centerIn: parent
        SequentialAnimation {
            running: { root.showNetworkIcon }
            loops: Animation.Infinite
            PropertyAction { target: networkIcon; property: "opacity"; value: 1 }
            PauseAnimation { duration: 500 }
            PropertyAction { target: networkIcon; property: "opacity"; value: 0.93 }
            PauseAnimation { duration: 500 }
        }
    }
    Text {
        id: statusLabel
        text: root.status
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: parent.height/36
        anchors.centerIn: parent
    }
    Text {
        id: informationLabel
        text: root.information
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.italic: true
        font.pixelSize: parent.height/54
        anchors.centerIn: parent
        anchors.verticalCenterOffset: parent.height/36
    }

    Row {
        id: buttonsRow
        spacing: parent.width/384
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -parent.width/640
        anchors.verticalCenterOffset: { if (informationLabel.text === '') parent.height/36; else parent.height/18 }
        Button {
            id: get
            width: screen.width/34.90
            height: screen.height/54
            label: "GET"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: screen.height/36
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: { root.requireConfirmation }
            enabled: { root.requireConfirmation }
            onClicked: {
                root.status = ""
                root.information = ""
                root.getFilesSIGNAL_QML()
            }
        }
        Button {
            id: skip
            width: screen.width/34.90
            height: screen.height/54
            label: "SKIP"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: screen.height/36
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: { root.requireConfirmation }
            enabled: { root.requireConfirmation }
            onClicked: { root.skipSIGNAL_QML() }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: { update.forceActiveFocus() }
}
