import QtQuick 2.3

Item {
    id: update

    Loader {
        id: loaderNetwork
        source: "Network.qml"
        asynchronous: true
        active: { root.showNetwork }
        anchors.fill: parent
    }

    Rectangle {
      id: backgroundLayer
      width: parent.width
      height: parent.height/4.32
      color: "black"
      visible: { !root.showNetwork && statusLabel.text != ""}
      anchors.centerIn: parent
    }
    Column {
        spacing: { if (informationLabel.text !="") -parent.height/72; else parent.height/216 }
        anchors.centerIn: parent

        Text {
            id: statusLabel
            text: root.status
            color: "white"
            font.family: pigFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: informationLabel
            text: root.information
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/23
            anchors.horizontalCenter: statusLabel.horizontalCenter
        }
        Row {
            id: buttonsRow
            spacing: screen.width/192
            anchors.horizontalCenter: statusLabel.horizontalCenter
            Button {
                id: get
                width: screen.width/25.26
                height: screen.height/36
                label: "GET"
                labelColor: Qt.rgba(0.1, 0.1, 0.1, 1)
                labelSize: screen.height/23
                labelInColor: "white"
                labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 1)
                visible: { root.requireConfirmation }
                enabled: { root.requireConfirmation }
                onClicked: {
                    root.status = ""
                    root.information = ""
                    root.get_files_qml_signal()
                }
            }
            Button {
                id: skip
                width: screen.width/22.32
                height: screen.height/36
                label: "SKIP"
                labelColor: Qt.rgba(0.1, 0.1, 0.1, 1)
                labelSize: screen.height/23
                labelInColor: "white"
                labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 1)
                visible: { root.requireConfirmation }
                enabled: { root.requireConfirmation }
                onClicked: { root.skip_qml_signal() }
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quit_qml_signal()
            event.accepted = true;
        }
    }

    Component.onCompleted: { update.forceActiveFocus() }
}
