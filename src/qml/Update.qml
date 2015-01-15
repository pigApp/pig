import QtQuick 2.4
import "global/"

Item {
    id: update

    Row {
        id: labelsRow
        spacing: parent.width/192
        anchors.centerIn: parent
        Text {
            id: statusLabel
            text: root.status
            color: { if(statusLabel.text === "FAIL") "red"; else "white" }
            font.family: globalFont.name
            font.bold: { statusLabel.text === "UPDATED" || statusLabel.text === "FAIL" }
            font.pixelSize: screen.height/23
        }
        Text {
            id: informationLabel
            text: root.information
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
    }
    Row {
        id: buttonsRow
        spacing: parent.width/192
        visible: { statusLabel.text === "UPDATE AVAILABLE" }
        enabled: { statusLabel.text === "UPDATE AVAILABLE" }
        anchors.top: labelsRow.bottom
        anchors.horizontalCenter: labelsRow.horizontalCenter
        anchors.horizontalCenterOffset: -parent.width/384
        Button {
            id: get
            width: screen.width/25.6
            height: screen.height/30.85
            label: "GET"
            labelColor: "white"
            labelFont: globalFont.name
            labelSize: screen.height/23
            onClicked: root.signal_qml_update_get()
        }
        Button {
            id: skip
            width: screen.width/22.58
            height: screen.height/30.85
            label: "SKIP"
            labelColor: "white"
            labelFont: globalFont.name
            labelSize: screen.height/23
            onClicked: root.signal_qml_update_skip()
        }
    }

    Network {
        visible: root.showNetwork
        anchors.fill: parent
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: update.forceActiveFocus()
}
