import QtQuick 2.2

Item {
    id: update

    Rectangle {
        id: waveLayer
        width: wave.width
        height: wave.height+8
        visible: root.showWave
        AnimatedImage {
            id: wave
            source: "qrc:/images/pig/wave.gif"
        }
        anchors.horizontalCenter: statusLabel.horizontalCenter
        anchors.verticalCenter: statusLabel.verticalCenter
        anchors.verticalCenterOffset: -30
    }
    Text {
        id: statusLabel
        text: root.status
        color: { if (waveLayer.visible) "white"; else Qt.rgba(0.1, 0.1, 0.1, 0.2) }
        font.family: pigFont.name
        font.bold: root.showWave
        font.pixelSize: 30
        anchors.centerIn: parent
    }
    Text {
        id: informationLabel
        text: root.information
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.italic: true
        font.pixelSize: 20
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 30
    }

    Row {
        id: buttonsRow
        spacing: 23
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -2
        anchors.verticalCenterOffset: { if (informationLabel.text == '') 50; else 70 }
        Button {
            id: accept
            width: 83
            height: 38
            label: "GET"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 50
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
            width: 93
            height: 38
            label: "SKIP"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 50
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: { root.requireConfirmation }
            enabled: { root.requireConfirmation }
            onClicked: { root.skipSIGNAL_QML() }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true;
        }
    }

    Component.onCompleted: { update.forceActiveFocus() }
}
