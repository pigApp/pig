import QtQuick 2.2

Item {
    id: update

    Text {
        id: statusInformationLabel
        text: "XXXXXXXXXXXXXXXXXXX"
        color: "black"
        font.family: pigLightFont.name
        font.italic: true
        font.pixelSize: 151
    }
    property string statusReader: root.status

    Row {
        id: statusRow
        spacing: 17
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 20

        Text {
            id: statusLabel
            text: root.status
            color: Qt.rgba(0.1, 0.1, 0.1, 0.13)
            font.family: pigFont.name
            font.pixelSize: 25
        }
        Text {
            id: statusInformationLabel
            text: root.statusInformation
            color: "black"
            font.family: pigLightFont.name
            font.italic: true
            font.pixelSize: 15
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
            anchors.verticalCenterOffset: 1.5
        }

        Row {
            id: buttonsRow
            spacing: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2.15
            Button {
                id: acceptUpdate
                label: "update"
                color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
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
                label: "skip"
                color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
                visible: root.requireAccept
                enabled: root.requireAccept
                onClicked: {
                    root.updateCancel()
                }
            }
            Button {
                id: restartUpdate
                label: "restart"
                color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
                visible: root.requireRestart
                enabled: root.requireRestart
                onClicked: { root.restart() } // TODO: Si os unix, mostrar mensaje que requiere autorizacion.
            }
        }
    }

    onStatusReaderChanged: {
        if(root.status.indexOf("update available") !== -1) {
            statusLabel.color = Qt.rgba(1, 1, 1, 1)
        }else if(root.status.indexOf("updating") !== -1) {
            statusLabel.color = Qt.rgba(1, 1, 1, 1)
            statusLabel.font.pixelSize = 11
        }else if(root.status.indexOf("UPDATE FAILED") !== -1 || root.status.indexOf("ERROR IN DATABASE") !== -1 || root.status.indexOf("FAILED TO UPDATE THE DATABASE") !== -1 || root.status.indexOf("PERMISSION DENIED") !== -1) {
            statusLabel.color = Qt.rgba(0.3, 0, 0, 1)
            statusInformationLabel.anchors.bottomMargin = 2.33
        }
    }
}
