import QtQuick 2.3
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
            color: { if(statusLabel.text === "FAIL") Qt.rgba(0.08, 0.08, 0.08, 1); else "white" }
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: informationLabel
            text: root.information
            color: { if(statusLabel.text === "FAIL") Qt.rgba(0.08, 0.08, 0.08, 1); else "white" }
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
        anchors.horizontalCenterOffset: -parent.width/192
        Button {
            id: accept
            width: screen.width/27.9
            height: screen.height/72
            label: "INSTALL"
            labelColor: "white"
            labelFont: globalFont.name
            labelBold: true
            labelSize: screen.height/54
            onClicked: root.signal_qml_accept_update()
        }
        Button {
            id: skip
            width: screen.width/49.23
            height: screen.height/72
            label: "SKIP"
            labelColor: "white"
            labelFont: globalFont.name
            labelBold: true
            labelSize: screen.height/54
            onClicked: root.signal_qml_skip_update()
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
