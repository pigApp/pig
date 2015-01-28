import QtQuick 2.4
import "global/"

Item {
    id: update

    Column {
        spacing: -parent.height/72
        anchors.centerIn: parent
        Row {
            id: rowState
            spacing: screen.width/192
            Text {
                id: labelStatus
                text: root.status
                color: "white"
                font.family: fontGlobal.name
                font.bold: { labelStatus.text === "UPDATED" }
                font.pixelSize: screen.height/23
                visible: { labelStatus.text !== "FAIL" }
            }
            Text {
                id: labelInformation
                text: root.information
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/23
            }
            Image {
                id: icon
                width: screen.width/58.18
                height: screen.height/32.72
                sourceSize.width: icon.width
                sourceSize.height: icon.height
                source: "qrc:/img_err"
                visible: { labelStatus.text === "FAIL" }
                anchors.verticalCenter: labelInformation.verticalCenter
                anchors.verticalCenterOffset: {
                    if (labelInformation.text === "GKSU·KDESU NEEDED")
                        screen.height/360
                    else
                        screen.height/635
                }
            }
        }
        Row {
            spacing: screen.width/192
            visible: { labelStatus.text === "UPDATE AVAILABLE" }
            enabled: { labelStatus.text === "UPDATE AVAILABLE" }
            anchors.horizontalCenter: rowState.horizontalCenter
            anchors.horizontalCenterOffset: -screen.width/384
            Button {
                id: btnGet
                label: "GET"
                onClicked: root.sig_qml_update_get()
            }
            Button {
                id: btnSkip
                label: "SKIP"
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
            root.sig_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: update.forceActiveFocus()
}
