import QtQuick 2.3

Item {
    id: finder
    x: root.xB

    property bool onCategoryFilter

    Text {
        id: welcomeLabel
        text: "WELCOME &nbsp; <font color='#161616'>CTRL H TO HELP</font></a>"
        color: "white"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        opacity: 0
        visible: root.init
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/154.28
    }
    NumberAnimation { target: welcomeLabel; running: root.init && selectors_loader.opacity === 1.0; properties: "opacity"; to: 1.0; duration: 1400; easing.type: Easing.InOutQuart }

    TextInput {
        id: userInput
        color: "white"
        font.family: finderFont.name
        font.capitalization: Font.AllUppercase
        font.letterSpacing: parent.width/480
        font.wordSpacing: -parent.width/384
        font.pixelSize: screen.height/18
        visible: { selectors_loader.opacity === 1.0 }
        enabled: { selectors_loader.opacity === 1.0 }
        maximumLength: 28
        cursorVisible: false
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -parent.width/112
        onAccepted: {
            root.input = userInput.text
            root.signal_qml_find(root.input, root.pornstar, root.category, root.quality, root.full, 0, true)
        }
        onCursorPositionChanged: {
            if (dbNullLabel.visible)
                dbNullLabel.visible = false
            userInput.visible = true
        }
        onCursorVisibleChanged: { if (userInput.cursorVisible) userInput.cursorVisible = false }
        onEnabledChanged: { if (userInput.enabled) userInput.forceActiveFocus() }
    }
    Text {
        id: dbNullLabel
        text: "NO RESULT"
        color: "gray"
        font.family: finderFont.name
        font.capitalization: Font.AllUppercase
        font.letterSpacing: parent.width/480
        font.wordSpacing: -parent.width/384
        font.pixelSize: screen.height/18
        opacity: 0.3
        visible: false
        anchors.centerIn: parent
    }

    Column {
        id: buttonsFiltersColumn
        spacing: parent.height/54
        opacity: 0
        anchors.left: parent.left
        anchors.leftMargin: parent.width/48.3
        anchors.bottom: parent.verticalCenter
        anchors.bottomMargin: -parent.height/44
        Button {
            id: categoryFilter
            width: screen.width/4.06
            height: screen.height/13.67
            label: "CATEGORY"
            labelColor: "white"
            labelSize: screen.height/10
            onClicked: {
                finder.state = "show_filter"
                onCategoryFilter = true
            }
        }
        Button {
            id: pornstarFilter
            width: screen.width/3.92
            height: screen.height/13.67
            label: "PORNSTAR"
            labelColor: "white"
            labelSize: screen.height/10
            onClicked: {
                finder.state = "show_filter"
                onCategoryFilter = false
            }
        }
    }
    Loader {
        id: filters_loader
        z: 2
        source: "Filters.qml"
        asynchronous: true
        active: false
        anchors.fill: parent
    }
    function filtersManager(label) {
        if (onCategoryFilter)
            root.category = label.toUpperCase()
        else
            root.pornstar = label.toUpperCase()
        root.signal_qml_find("", root.pornstar, root.category, "", "", 0, true)
    }

    Loader {
        id: selectors_loader
        source: "Selectors.qml"
        asynchronous: true
        active: true
        opacity: 0
        anchors.fill: parent
    }

    states: [
        State {
            name: "show"
        },
        State {
            name: "hide"
        },
        State {
            name: "show_filter"
            PropertyChanges { target: root; init: false; restoreEntryValues: false }
        },
        State {
            name: "hide_filter"
        },
        State {
            name: "hide_filter_finder"
        }
    ]
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {
                NumberAnimation { duration: 100 }
                ParallelAnimation {
                    NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: selectors_loader; properties: "opacity"; to: 1.0; duration: 10; easing.type: Easing.InOutQuart }
                    PropertyAction { target: userInput; property: "text"; value: root.input }
                }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: userInput; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: selectors_loader; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 0; duration: 50; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 1100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filters_loader; property: "source"; value: "" }
                PropertyAction { target: selectors_loader; property: "source"; value: "" }
                PropertyAction { target: screen; property: "state"; value: "show_viewer" }
            }
        },
        Transition {
            to: "show_filter"
            SequentialAnimation {
                PropertyAction { target: filters_loader; property: "active"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 96; duration: 2000; easing.type: Easing.OutQuart }
                }
            }
        },
        Transition {
            to: "hide_filter"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 2100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filters_loader; property: "active"; value: false }
                PropertyAction { target: userInput; property: "focus"; value: true }
            }
        },
        Transition {
            to: "hide_filter_finder"
            SequentialAnimation {
                PropertyAction { target: userInput; property: "opacity"; value: 0 }
                PropertyAction { target: dbNullLabel; property: "opacity"; value: 0 }
                PropertyAction { target: buttonsFiltersColumn; property: "opacity"; value: 0 }
                PropertyAction { target: selectors_loader; property: "source"; value: "" }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 1100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filters_loader; property: "source"; value: "" }
                PropertyAction { target: screen; property: "state"; value: "show_viewer" }
            }
        }
    ]

    Keys.onPressed: {
        root.init = false
        if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
            if (!filters_loader.active) {
                screen.state = "show_help"
                event.accepted = true
            }
        } else if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier)) {
            if (!filters_loader.active) {
                screen.state = "show_password"
            }
            event.accepted = true
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true
        }
    }

    Connections {
        target: cppSignals
        onSignal_ret_db: {
            if (nFilms === 0) {
                userInput.visible = false
                dbNullLabel.visible = true
            }
        }
    }

    Component.onCompleted: { finder.state = "show" }
}
// Tabs hechos.
