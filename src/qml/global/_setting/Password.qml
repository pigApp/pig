import QtQuick 2.4

Rectangle {
    id: password
    color: "black"

    property bool onSet

    TextInput {
        id: userInput
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: { if (onSet) screen.height/74.2; else screen.height/43.2 }
        echoMode: TextInput.Password
        maximumLength: 16
        cursorVisible: false
        anchors.centerIn: parent
        onCursorVisibleChanged: { if (cursorVisible) cursorVisible = false }
        onCursorPositionChanged: {
            if (password.color == "#ff0000" && enabled)
                password.color = "black"
            if (text === "")
                label.visible = true
            else
                label.visible = false
        }
        onFocusChanged: {
            if (onSet && enabled) {
                if (focus)
                    password.color = "#202020"
                else
                    password.color = "black"
            }
        }
        onAccepted: {
            if (text !== "") {
                if (onSet) {
                    enabled = false
                    cpp.password_handler(false, text, false, true)
                    setting.forceActiveFocus()
                } else {
                    cpp.password_handler(false, text, true, false)
                }
            }
        }
        Keys.onPressed: {
            if (event.key === Qt.Key_Escape && onSet) {
                screen.state = "hide_setting"
                event.accepted = true
            } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
                cpp.quit()
                event.accepted = true
            }
        }
    }

    Row {
        spacing: screen.width/192
        anchors.centerIn: parent
        Text {
            id: label
            text: { if (onSet) "INTRO NEW PASSWORD"; else "PASSWORD" }
            color: "white"
            font.family: fontGlobal.name
            font.bold: onSet
            font.pixelSize: { if (onSet) screen.height/54; else screen.height/23 }
        }
        Text {
            id: labelInformation
            text: "CHECK PERMISSIONS"
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/54
            visible: { label.text === "FAIL" }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: { if (userInput.enabled) userInput.focus = true }
    }

    Connections {
        target: cpp
        onSig_ret_password: {
            if (success) {
                color = Qt.rgba(0, 0.28, 0.047, 1)
                userInput.text = ""
                label.text = "DONE" 
                label.visible = true
            } else {
                color = "red"
                if (onSet) {
                    userInput.text = ""
                    label.text = "FAIL"
                    label.visible = true
                }
            }
        }
    }

    Component.onCompleted: userInput.forceActiveFocus()
}
// Tabs hechos.
