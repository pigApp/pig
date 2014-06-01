import QtQuick 2.2

Item {
    id: update

    property string statusReader: root.status

    Row {
        id: statusRow
        spacing: 10
        anchors.centerIn: parent
        Text {
            id: statusLabel
            text: root.status
            color: Qt.rgba(0.2, 0.2, 0.2, 0.4)
            font.family: pigFont.name
            font.capitalization: Font.AllUppercase
            font.pixelSize: 25
        }
        Text {
            id: statusInformationLabel
            text: root.statusInformation
            color: Qt.rgba(0.5, 0.5, 0.5, 0.15)
            font.family: pigFont.name
            font.capitalization: Font.AllUppercase
            font.italic: true
            font.pixelSize: 13
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 20
        }
        Image {
            id: spinner
            width: 26
            height: 26
            source: "qrc:/images/spinner.png" 
            opacity: 0.5
            visible: showSpinner
            property bool on: showSpinner
            NumberAnimation on rotation { running: spinner.on; from: 0; to: 360; loops: Animation.Infinite; duration: 400 }
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 1
        }

        Row {
            id: buttonsRow
            spacing: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2.15
            Button {
                id: acceptUpdate
                label: "update"
                color: Qt.rgba(0.1 ,0.1 ,0.1 , 0.2)
                visible: showDecisionButton && !restart
                enabled: showDecisionButton && !restart
                onClicked: {
                    root.status = ''
                    root.statusInformation  = ''
                    root.updateAccept()
                }
            }
            Button {
                id: cancelUpdate
                label: "skip"
                color: Qt.rgba(0.1 ,0.1 ,0.1 , 0.2)
                visible: showDecisionButton && !restart
                enabled: showDecisionButton && !restart
                onClicked: {
                    root.status = ''
                    root.statusInformation  = ''
                    root.updateCancel()
                }
            }
            Button {
                id: restartUpdate
                label: "restart"
                color: Qt.rgba(0.1 ,0.1 ,0.1 , 0.2)
                visible: restart
                enabled: restart
                onClicked: { root.updateRestart() }
            }
            Button {
                id: authorizeUpdate
                label: "authorize"
                color: Qt.rgba(0.1 ,0.1 ,0.1 , 0.2)
                visible: authorize
                enabled: authorize
                onClicked: { root.updateRestart() }
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
