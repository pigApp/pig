import QtQuick 2.4

Rectangle {
    id: password
    color: "black"

    TextInput {
        id: userInput
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/43.2
        echoMode: TextInput.Password
        maximumLength: 16
        cursorVisible: false
        anchors.centerIn: parent
        onCursorVisibleChanged: { if (cursorVisible) cursorVisible = false }
        onCursorPositionChanged: {
            if ((password.color == "#ff0000") && root.askPassword)
                password.color = "black"
            if (text === "")
                label.visible = true
            else
                label.visible = false
        }
        onAccepted: {
            if (text !== "") {
                if (root.askPassword) {
                    cpp.password_handler(false, text, true, false)
                } else {
                    enabled = false
                    cpp.password_handler(false, text, false, true)
                }
            }
        }
        Keys.onPressed: {
            if ((event.key === Qt.Key_Escape) && (!root.askPassword)) {
                screen.state = "hide_password"
                event.accepted = true
            } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
                cpp.quit()
                event.accepted = true;
            }
        }
    }
    Row {
        spacing: screen.width/192
        anchors.centerIn: parent
        Text {
            id: label
            text: "INTRO PASSWORD"
            color: "white"
            font.family: fontGlobal.name
            font.bold: { text === "FAIL" }
            font.pixelSize: screen.height/23
        }
        Text {
            id: labelInformation
            text: "CHECK PERMISSIONS"
            color: "white"
            font.family: fontGlobal.name
            font.pixelSize: screen.height/23
            visible: { label.text === "FAIL" }
        }
    }

    MouseArea {
        onClicked: { userInput.focus = true }
        anchors.fill: parent
    }

    Timer {
        id: delayHide
        running: false
        repeat: false
        interval: 5000
        onTriggered: { screen.state = "hide_password" }
    }

    Connections {
        target: cpp
        onSig_ret_password: {
            if (success) {
                if (root.askPassword)
                    root.askPassword = false
                screen.state = "hide_password"
            } else {
                color = "red"
                if (!root.askPassword) {
                    userInput.text = ""
                    label.text = "FAIL"
                    label.visible = true
                    delayHide.start()
                }
            }
        }
    }

    Component.onCompleted: userInput.forceActiveFocus()
}
// Tabs hechos.
