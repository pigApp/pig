import QtQuick 2.3

Item {
    id: update

    Loader {
        id: loaderNetwork
        source: "Network.qml"
        asynchronous: true
        active: root.showNetwork
        anchors.fill: parent
    }

    Rectangle {
      id: backgroundLayer
      width: parent.width
      height: parent.height/4.32
      color: "black"
      visible: { statusLabel.text !== "" && statusLabel.text !== "GETTING UPDATE"}
      anchors.centerIn: parent
    }
    Column {
        spacing: -parent.height/72
        anchors.centerIn: parent
        anchors.verticalCenterOffset: { if (statusLabel.text === "GETTING UPDATE") screen.height/8.3; else 0 }

        Text {
            id: statusLabel
            text: root.status
            color: { if (statusLabel.text === "GETTING UPDATE") "black"; else "white" }
            font.bold: { statusLabel.text === "GETTING UPDATE" }
            font.family: pigFont.name
            font.pixelSize: { if (statusLabel.text === "GETTING UPDATE") screen.height/54; else screen.height/23 }
        }
        Text {
            id: informationLabel
            text: root.information
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/23
            anchors.horizontalCenter: statusLabel.horizontalCenter
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: update.forceActiveFocus()
}
