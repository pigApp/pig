import QtQuick 2.4

Rectangle {
    id: help
    color: Qt.rgba(0, 0, 0, 0.7)

    Loader {
        id: loader
        source: root.helpSource
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
    }

    Keys.onPressed: {
        loader.source = ""
        screen.state = "hide_help"
        event.accepted = true
    }

    Component.onCompleted: help.forceActiveFocus()
}
// Tabs hechos.
