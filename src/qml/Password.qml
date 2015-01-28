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
        onCursorVisibleChanged: { if (userInput.cursorVisible) userInput.cursorVisible = false }
        onCursorPositionChanged: {
            if (icon.visible)
                icon.visible = false
            if (userInput.text === "")
                label.opacity = 1
            else
                label.opacity = 0
        }
        onAccepted: {
            if (userInput.text !== "") {
                if (root.askPassword) {
                    root.sig_qml_password_handler(false, userInput.text, true, false)
                } else {
                    root.sig_qml_password_handler(false, userInput.text, false, true)
                    userInput.enabled = false
                }
            }
        }
        Keys.onPressed: {
            if ((event.key === Qt.Key_Escape) && (!root.askPassword)) {
                screen.state = "hide_password"
                event.accepted = true
            } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
                root.sig_qml_quit()
                event.accepted = true;
            }
        }
    }
    Text {
        id: label
        text: "INTRO PASSWORD"
        color: "white"
        font.family: fontGlobal.name
        font.pixelSize: screen.height/23
        anchors.centerIn: parent
    }
    Image {
        id: icon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: icon.width
        sourceSize.height: icon.height
        source: "qrc:/img_err"
        visible: false
        anchors.left: userInput.right
        anchors.leftMargin: parent.width/192
        anchors.verticalCenter: userInput.verticalCenter
        anchors.verticalCenterOffset: parent.height/360
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
        target: cppSignals
        onSig_ret_password: {
            if (success) {
                screen.state = "hide_password"
            } else {
                if (root.askPassword) {
                    icon.visible = true
                } else {
                    userInput.text = ""
                    label.text = "CHECK PERMISSIONS"
                    label.opacity = 1
                    icon.anchors.left = label.right
                    icon.visible = true
                    delayHide.start()
                }
            }
        }
    }

    Component.onCompleted: userInput.forceActiveFocus()
}
// Tabs hechos.
