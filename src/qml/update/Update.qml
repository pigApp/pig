import QtQuick 2.4

import "../global/"

Item {
    id: update

    Column {
        id: columnStatus
        spacing: -parent.height/72
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
                font.bold: { text === "UPDATED" || text === "FAIL" }
                font.pixelSize: screen.height/23
                onTextChanged: {
                    if (text === "UPDATE AVAILABLE")
                        update.state = "available"
                    else if (text === "UPDATED")
                        update.state = "success"
                    else if (text === "FAIL")
                        update.state = "err"
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
            spacing: screen.width/384
            enabled: { label.text === "UPDATE AVAILABLE" }
            visible: { label.text === "UPDATE AVAILABLE" }
            anchors.horizontalCenter: rowStatus.horizontalCenter
            Button {
                id: btnGet
                label: "GET"
                labelSize: screen.height/23
                onClicked: {
                    columnStatus.enabled = false
                    columnStatus.visible = false
                    root.sig_qml_update_get()
                }
            }
            Button {
                id: btnSkip
                label: "SKIP"
                labelSize: screen.height/23
                onClicked: root.sig_qml_update_skip()
            }
        }
    }

    Network {
        visible: root.network
        anchors.fill: parent
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
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
            PropertyChanges { target: root; screenR: 0; restoreEntryValues: false }
            PropertyChanges { target: root; screenG: 0.28; restoreEntryValues: false }
            PropertyChanges { target: root; screenB: 0.047; restoreEntryValues: false }
            PropertyChanges { target: root; screenA: 1; restoreEntryValues: false }
            PropertyChanges { target: columnStatus; visible: true; restoreEntryValues: false }
        },
        State {
            name: "err"
            PropertyChanges { target: root; screenR: 1; restoreEntryValues: false }
            PropertyChanges { target: root; screenG: 0; restoreEntryValues: false }
            PropertyChanges { target: root; screenB: 0; restoreEntryValues: false }
            PropertyChanges { target: root; screenA: 0.9; restoreEntryValues: false }
            PropertyChanges { target: columnStatus; visible: true; restoreEntryValues: false }
        }
    ]
    transitions: [
        Transition {
            to: "available"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenR"; to: 1; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenG"; to: 0.84; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenB"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.2; duration: 600; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: columnStatus; property: "visible"; value: true }
                PropertyAction { target: columnStatus; property: "enabled"; value: true }
            }
        }
    ]

    Component.onCompleted: forceActiveFocus()
}
// Tabs hechos.
