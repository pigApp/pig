import QtQuick 2.4

Rectangle {
    id: help
    color: Qt.rgba(1, 1, 1, 0.5)

    Loader {
        id: loader
        source: { if (root.help_finder) "_help/Help_finder.qml"; else "_help/Help_viewer.qml" }
        asynchronous: true
        visible: { status === Loader.Ready }
        anchors.fill: parent
    }

    Keys.onPressed: {
        loader.source = ""
        if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        } else {
            screen.state = "hide_help"
            event.accepted = true
        }
    }

    Component.onCompleted: help.forceActiveFocus()
}
// Tabs hechos.
