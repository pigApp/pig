import QtQuick 2.4
import "_setting/"

Rectangle {
    id: setting
    color: "black"

    Row {
        anchors.top: parent.top
        anchors.left: parent.left
        About {
            id: about
            width: screen.width/4
            height: screen.height
        }
        Password {
            id: password
            color: "#202020"
            width: screen.width/4
            height: screen.height
            set: true
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            screen.state = "hide_setting"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }
}
// Tabs hechos.
