import QtQuick 2.4

Item {
    id: update

    Column {
        id: columnStatus
        spacing: -parent.height/108
        enabled: false
        visible: false
        anchors.centerIn: parent
        Row {
            id: rowStatus
            spacing: screen.width/192
            Text {
                id: label
                text: root.status
                color: "white"
                font.family: fontGlobal.name
                font.bold: { text === "DONE" || text === "FAIL" }
                font.pixelSize: screen.height/23
                onTextChanged: {
                    if (text === "UPDATE AVAILABLE")
                        update.state = "available"
                    else if (text === "DONE")
                        update.state = "success"
                    else if (text === "FAIL")
                        update.state = "error"
                }
            }
            Text {
                id: labelInformation
                text: root.information
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
        }
        Row {
            spacing: screen.width/192
            enabled: { label.text === "UPDATE AVAILABLE" }
            visible: { label.text === "UPDATE AVAILABLE" }
            anchors.horizontalCenter: rowStatus.horizontalCenter
            Button {
                id: btnGet
                label: "GET"
                labelColor: "white"
                labelSize: screen.height/23
                labelColorIn: "green"
                onClicked: {
                    columnStatus.enabled = false
                    columnStatus.visible = false
                    root.sig_qml_update_get()
                }
            }
            Button {
                id: btnSkip
                label: "SKIP"
                labelColor: "white"
                labelSize: screen.height/23
                labelColorIn: "gold"
                onClicked: root.sig_qml_update_skip()
            }
        }
    }

    Network {
        visible: root.network
        anchors.fill: parent
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q)
            && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    states: [
        State {
            name: "available"
        },
        State {
            name: "success"
            PropertyChanges { target: columnStatus; visible: true; restoreEntryValues: false }
        },
        State {
            name: "error"
            PropertyChanges { target: root; screenR: 1; restoreEntryValues: false }
            PropertyChanges { target: root; screenG: 0; restoreEntryValues: false }
            PropertyChanges { target: root; screenB: 0; restoreEntryValues: false }
            PropertyChanges { target: root; screenA: 0.4; restoreEntryValues: false }
            PropertyChanges { target: columnStatus; visible: true; restoreEntryValues: false }
        }
    ]
    transitions: [
        Transition {
            to: "available"
            SequentialAnimation {
                PropertyAction { target: columnStatus; property: "visible"; value: true }
                PropertyAction { target: columnStatus; property: "enabled"; value: true }
            }
        }
    ]

    Component.onCompleted: forceActiveFocus()
}
// Tabs hechos.
