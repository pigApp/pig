import QtQuick 2.3

Item {
    id: update

    Loader {
        id: network_loader
        source: "global/Network.qml"
        asynchronous: true
        active: root.showNetwork
        anchors.fill: parent
    }

    Row {
        id: labelsRow
        spacing: parent.height/192
        anchors.centerIn: parent
        Text {
            id: statusLabel
            text: root.status
            color: "white"
            font.family: globalFont.name
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

    Keys.onPressed: {
        if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true;
        }
    }

    Component.onCompleted: update.forceActiveFocus()
}
