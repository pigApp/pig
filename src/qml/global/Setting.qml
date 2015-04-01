import QtQuick 2.4
import "_setting/"

Item {
    id: setting

    Row {
        Column {
            id: column_a
            About {
                id: about
                width: screen.width/3
                height: screen.height/2
            }
            Password {
                id: password
                width: screen.width/3
                height: screen.height/2
                onSet: true
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            screen.state = "hide_setting"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q)
            && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }
}
// Tabs hechos.
