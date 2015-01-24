import QtQuick 2.4

Rectangle {
    id: password
    color: "black"

    TextInput {
        id: user
        color: "white"
        font.family: globalFont.name
        font.pixelSize: screen.height/43.2
        echoMode: TextInput.Password
        maximumLength: 16
        cursorVisible: false
        anchors.centerIn: parent
        onCursorVisibleChanged: { if (user.cursorVisible) user.cursorVisible = false }
        onCursorPositionChanged: {
            if (failIcon.visible)
                failIcon.visible = false
            if (user.text === "")
                label.opacity = 1
            else
                label.opacity = 0
        }
        onAccepted: {
            if (user.text !== "") {
                if (root.askPassword) {
                    root.signal_qml_password_handler(user.text, false, true, false)
                } else {
                    root.signal_qml_password_handler(user.text, false, false, true)
                    user.enabled = false
                }
            }
        }
        Keys.onPressed: {
            if (event.key === Qt.Key_Escape && !root.askPassword) {
                screen.state = "hide_password"
                event.accepted = true
            } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                root.signal_qml_quit()
                event.accepted = true;
            }
        }
    }
    Text {
        id: label
        text: "INTRO PASSWORD"
        color: "white"
        font.family: globalFont.name
        font.pixelSize: screen.height/23
        anchors.centerIn: parent
    }
    Image {
        id: failIcon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: 33
        sourceSize.height: 33
        source: "qrc:/img-err"
        visible: false
        anchors.left: user.right
        anchors.leftMargin: parent.width/192
        anchors.verticalCenter: user.verticalCenter
        anchors.verticalCenterOffset: parent.height/360
    }

    MouseArea {
        onClicked: { user.focus = true }
        anchors.fill: parent
    }

    Timer {
        id: hideDelay
        running: false
        repeat: false
        interval: 5000
        onTriggered: { screen.state = "hide_password" }
    }

    Connections {
        target: cppSignals
        onSignal_ret_password: {
            if (success) {
                screen.state = "hide_password"
            } else {
                if (root.askPassword) {
                    failIcon.visible = true
                } else {
                    user.text = ""
                    label.text = "CHECK PERMISSIONS"
                    label.opacity = 1
                    failIcon.anchors.left = label.right
                    failIcon.visible = true
                    hideDelay.start()
                }
            }
        }
    }

    Component.onCompleted: user.forceActiveFocus()
}
// Tabs hechos.
