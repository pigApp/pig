import QtQuick 2.4
import "global/"

Item {
    id: update

    Column {
        id: columnStatus
        spacing: -parent.height/72
        visible: false
        enabled: false
        anchors.centerIn: parent
        Row {
            id: rowStatus
            spacing: screen.width/192
            Text {
                id: label
                text: root.status
                color: "white"
                font.family: fontGlobal.name
                font.bold: { label.text === "UPDATED" || label.text === "FAIL" }
                font.pixelSize: screen.height/23
                onTextChanged: {
                    if (label.text === "UPDATE AVAILABLE")
                        update.state = "available"
                    else if (label.text === "UPDATED")
                        update.state = "success"
                    else if (label.text === "FAIL")
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
            spacing: screen.width/192
            visible: { label.text === "UPDATE AVAILABLE" }
            enabled: { label.text === "UPDATE AVAILABLE" }
            anchors.horizontalCenter: rowStatus.horizontalCenter
            anchors.horizontalCenterOffset: -screen.width/384
            Button {
                id: btnGet
                label: "GET"
                labelBold: true
                labelSize: screen.height/23
                mouseAreaMargin: screen.height/108
                onClicked: {
                    columnStatus.visible = false
                    columnStatus.enabled = false
                    root.sig_qml_update_get()
                }
            }
            Button {
                id: btnSkip
                label: "SKIP"
                labelBold: true
                labelSize: screen.height/23
                mouseAreaMargin: screen.height/108
                onClicked: root.sig_qml_update_skip()
            }
        }
    }

    Network {
        visible: root.showNetwork
        anchors.fill: parent
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true;
        }
    }

    states: [
        State {
            name: "available"
        },
        State {
            name: "success"
        },
        State {
            name: "err"
        }
    ]
    transitions: [
        Transition {
            to: "available"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenR"; to: 1; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenG"; to: 1; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenB"; to: 0; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.2; duration: 300; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: columnStatus; property: "visible"; value: true }
                PropertyAction { target: columnStatus; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "success"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenR"; to: 0; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenG"; to: 0.28; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenB"; to: 0.047; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 1; duration: 300; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: columnStatus; property: "visible"; value: true }
            }
        },
        Transition {
            to: "err"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenR"; to: 1; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenG"; to: 0; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenB"; to: 0; duration: 300; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.9; duration: 300; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: columnStatus; property: "visible"; value: true }
            }
        }
    ]

    Component.onCompleted: update.forceActiveFocus()
}
// Tabs hechos.
