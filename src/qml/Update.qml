import QtQuick 2.4
import "global/"

Item {
    id: update

    Column {
        spacing: -parent.height/72
        anchors.centerIn: parent
        Row {
            id: statesRow
            spacing: screen.width/192
            Text {
                id: statusLabel
                text: root.status
                color: "white"
                font.family: globalFont.name
                font.bold: { statusLabel.text === "UPDATED" }
                font.pixelSize: screen.height/23
                visible: { statusLabel.text !== "FAIL" }
            }
            Text {
                id: informationLabel
                text: root.information
                color: "white"
                font.family: globalFont.name
                font.pixelSize: screen.height/23
            }
            Image {
                id: failIcon
                width: screen.width/58.18
                height: screen.height/32.72
                sourceSize.width: failIcon.width
                sourceSize.height: failIcon.height
                source: "qrc:/img-err"
                visible: { statusLabel.text === "FAIL" }
                anchors.verticalCenter: informationLabel.verticalCenter
                anchors.verticalCenterOffset: {
                    if (informationLabel.text === "GKSU·KDESU NEEDED")
                        screen.height/360
                    else
                        screen.height/635
                }
            }
        }
        Row {
            id: buttonsRow
            spacing: screen.width/192
            visible: { statusLabel.text === "UPDATE AVAILABLE" }
            enabled: { statusLabel.text === "UPDATE AVAILABLE" }
            anchors.horizontalCenter: statesRow.horizontalCenter
            anchors.horizontalCenterOffset: -screen.width/384
            Button {
                id: get
                label: "GET"
                onClicked: root.signal_qml_update_get()
            }
            Button {
                id: skip
                label: "SKIP"
                onClicked: root.signal_qml_update_skip()
            }
        }
    }

    Network {
        visible: root.showNetwork
        anchors.fill: parent
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: update.forceActiveFocus()
}
