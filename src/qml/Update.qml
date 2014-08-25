import QtQuick 2.2

Item {
    id: update

    Image {
        id: network
        width: parent.width/5.36
        height: parent.height/3.01
        source: "qrc:/images/pig/network.png"
        sourceSize.width: 358
        sourceSize.height: 358
        visible: { root.showNetwork }
        anchors.centerIn: parent
        SequentialAnimation {
            running: { root.showNetwork }
            loops: Animation.Infinite
            PropertyAction { target: witnessStatusLabel; property: "visible"; value: "true" }
            PauseAnimation { duration: 500 }
            PropertyAction { target: witnessStatusLabel; property: "visible"; value: "false" }
            PauseAnimation { duration: 500 }
        }
    }
    Text {
        id: statusLabel
        text: root.status
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.pixelSize: parent.height/36
        anchors.centerIn: parent
    }
    Text {
        id: witnessStatusLabel
        text: "•"
        color: "white"
        font.family: pigFont.name
        font.pixelSize: parent.height/43.2
        visible: { !root.showNetwork }
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: parent.width/3500
        anchors.verticalCenterOffset: parent.height/130
    }
    Text {
        id: informationLabel
        text: root.information
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.italic: true
        font.pixelSize: parent.height54 //20
        anchors.centerIn: parent
        anchors.verticalCenterOffset: parent.height/36 //30
    }

    Row {
        id: buttonsRow
        spacing: parent.width/83.47 //23
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -parent.width/960 //-2
        anchors.verticalCenterOffset: { if (informationLabel.text === '') 50; else 70 }
        Button {
            id: accept
            width: screen.width/23.13 //83
            height: screen.height/28.42 //38
            label: "GET"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: screen.height/21.5 //50
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
            width: screen.width/20.64 //93
            height: screen.height/28.42 //38
            label: "SKIP"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: screen.height/21.5 //50
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
