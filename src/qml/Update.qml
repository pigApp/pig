import QtQuick 2.2

Item {
    id: update

    Row {
        id: statusRow
        spacing: 17
        anchors.centerIn: parent

        Text {
            id: statusLabel
            text: root.status
            color: { if(root.status != "SEEKING UPDATE") Qt.rgba(0.1, 0.1, 0.1, 0.2); else Qt.rgba(0.1, 0.1, 0.1, 0.1); }
            font.family: pigFont.name
            font.pixelSize: 30
        }
        Image {
            id: spinner
            width: 25
            height: 25
            source: "qrc:/images/spinner.png"
            fillMode: Image.PreserveAspectCrop
            clip: true
            visible: showSpinner
            property bool on: showSpinner
            NumberAnimation on rotation { running: spinner.on; from: 0; to: 360; loops: Animation.Infinite; duration: 1200 }
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    Text {
        id: informationLabel
        text: root.information
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.italic: true
        font.pixelSize: 30
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
            width: 90
            height: 38
            label: "GET"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 55
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: root.requireConfirmation
            enabled: root.requireConfirmation
            onClicked: {
                root.status = ''
                root.information = ''
                root.getFiles()
            }
        }
        Button {
            id: skip
            width: 102
            height: 38
            label: "SKIP"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 55
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: root.requireConfirmation
            enabled: root.requireConfirmation
            onClicked: { root.skip() }
        }
        Button {
            id: restart
            width: 140
            height: 38
            label: "RESTART"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 55
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: root.requireRestart
            enabled: root.requireRestart
            onClicked: { root.restart() } // TODO: Si os unix, mostrar mensaje que requiere autorizacion.
        }
    }
}
