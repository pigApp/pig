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
            color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
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
        id: statusInformationLabel
        text: root.statusInformation
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.italic: true
        font.pixelSize: 30
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 30
    }

    Row {
        id: buttonsRow
        spacing: 15
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -3
        anchors.verticalCenterOffset: { if(statusInformationLabel.text == '') 30; else 50 }
        Button {
            id: acceptUpdate
            width: 50
            height: 23
            label: "GET"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 30
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: root.requireAccept
            enabled: root.requireAccept
            onClicked: {
                root.status = ''
                root.statusInformation  = ''
                root.getFiles()
            }
        }
        Button {
            id: cancelUpdate
            width: 55
            height: 23
            label: "SKIP"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 30
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: root.requireAccept
            enabled: root.requireAccept
            onClicked: { root.updateCancel() }
        }
        Button {
            id: restartUpdate
            width: 100
            height: 25
            label: "RESTART"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            labelBold: true
            labelSize: 30
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.3)
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.2)
            visible: root.requireRestart
            enabled: root.requireRestart
            onClicked: { root.restart() } // TODO: Si os unix, mostrar mensaje que requiere autorizacion.
        }
    }
}
