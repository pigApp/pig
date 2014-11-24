import QtQuick 2.3

Item {
    id: finder

    property string activeFilter

    Text {
        id: userInputLabel
        text: "TYPE THE TITLE TO FIND"
        color: "gray"
        font.family: finderFont.name
        font.capitalization: Font.AllUppercase
        font.letterSpacing: parent.width/480
        font.wordSpacing: -parent.width/384
        font.pixelSize: screen.height/18
        opacity: 0.3
        visible: false
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.height/360
    }
    TextInput {
        id: userInput
        color: "gray"
        font.family: finderFont.name
        font.capitalization: Font.AllUppercase
        font.letterSpacing: parent.width/480
        font.wordSpacing: -parent.width/384
        font.pixelSize: screen.height/18
        visible: { loaderSelectors.opacity === 1.0 && screen.state !== "showSetPassword" }
        enabled: { loaderSelectors.opacity === 1.0 && screen.state !== "showSetPassword" }
        maximumLength: 28
        cursorVisible: false
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -parent.width/112
        anchors.verticalCenterOffset: -parent.height/360
        onAccepted: {
            root.input = userInput.text
            root.signal_qml_find(root.input, root.pornstar, root.category, root.quality, root.full, 0, true)
        }
        onCursorPositionChanged: {
            if (noResultLabel.visible)
                noResultLabel.visible = false
            userInputLabel.text = ""
            userInput.visible = true
        }
        onCursorVisibleChanged: { if (userInput.cursorVisible) userInput.cursorVisible = false }
    }

    Text {
        id: noResultLabel
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
        anchors.verticalCenterOffset: -parent.height/360
    }

    Column {
        id: buttonsFiltersColumn
        spacing: parent.height/54
        visible: { screen.state !== "showSetPassword" }
        enabled: { screen.state !== "showSetPassword" }
        opacity: 0
        anchors.left: parent.left
        anchors.leftMargin: parent.width/24.61
        anchors.bottom: parent.verticalCenter
        anchors.bottomMargin: -parent.height/60
        onEnabledChanged: { if (enabled) userInput.forceActiveFocus() }
        Button {
            id: categoryFilter
            width: screen.width/4.06
            height: screen.height/13.67
            label: "CATEGORY"
            labelSize: screen.height/10
            labelColor: "black"
            labelBold: true
            labelInColor: "red"
            labelOutColor: "black"
            onClicked: {
                finder.state = "showFilter"
                activeFilter = "CATEGORY"
            }
        }
        Button {
            id: pornstarFilter
            width: screen.width/3.92
            height: screen.height/13.67
            label: "PORNSTAR"
            labelSize: screen.height/10
            labelColor: "black"
            labelBold: true
            labelInColor: "red"
            labelOutColor: "black"
            onClicked: {
                finder.state = "showFilter"
                activeFilter = "PORNSTAR"
            }
        }
    }
    Loader {
        id: loaderFilter
        z: 2
        source: "Filters.qml"
        asynchronous: true
        active: false
        anchors.fill: parent
    }
    function filtersManager(filter, label) {
        if (filter === 'categoryFilter')
            root.category = label.toUpperCase()
        else
            root.pornstar = label.toUpperCase()
        noResultLabel.visible = false
        root.signal_qml_find("", root.pornstar, root.category, "", "", 0, true)
    }

    Loader {
        id: loaderSelectors
        source: "Selectors.qml"
        asynchronous: true
        active: true
        visible: { screen.state !== "showSetPassword" }
        enabled: { screen.state !== "showSetPassword" }
        opacity: 0
        anchors.fill: parent
    }

    MouseArea {
        id: mouseArea
        enabled: { root.showUserInputLabel && loaderSelectors.visible }
        visible: { root.showUserInputLabel && loaderSelectors.visible }
        hoverEnabled: true
        onPositionChanged: {
            if (userInput.text === "") {
                userInputLabel.visible = true
                root.showUserInputLabel = false
            }
        }
        anchors.fill: parent
    }

    Timer {
        id: focusDelay
        running: false
        repeat: false
        interval: 1200
        onTriggered: userInput.forceActiveFocus()
    }

    states: [
        State {
            name: "show"
        },
        State {
            name: "hide"
            when: root.hideFinder_showOutput
        },
        State {
            name: "hideWelcome"
        },
        State {
            name: "hideAll"
            when: root.hideFilters_hideFinder_showOutput
        },
        State {
            name: "showFilter"
        },
        State {
            name: "hideFilter"
        },
        State {
            name: "showHelp"
        },
        State {
            name: "hideHelp"
            when: root.hideHelp
        }
    ]
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {
                NumberAnimation { duration: 100 }
                NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 1.0; duration: 200; easing.type: Easing.InOutQuart } 
                ParallelAnimation {
                    NumberAnimation { target: loaderSelectors; properties: "opacity"; to: 1.0; duration: 60; easing.type: Easing.InOutQuart }
                    PropertyAction { target: userInput; property: "text"; value: root.input }
                }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                ParallelAnimation {
                    PropertyAction { target: userInputLabel; properties: "visible"; value: false }
                    NumberAnimation { target: userInput; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: loaderSelectors; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 0; duration: 60; easing.type: Easing.InOutQuart }
                NumberAnimation { duration: 100 }
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                PropertyAction { target: loaderSelectors; property: "source"; value: "" }
                PropertyAction { target: loaderSelectors; property: "active"; value: false }
                PropertyAction { target: root_loader_B; property: "source"; value: "Output.qml" }
                PropertyAction { target: root_loader_A; property: "source"; value: "Network.qml" }
                PropertyAction { target: root; property: "hideFinder_showOutput"; value: false }
            }
        },
        Transition {
            to: "hideWelcome"
            SequentialAnimation {
                PropertyAction { target: finder; property: "x"; value: -screen.width }
                PropertyAction { target: buttonsFiltersColumn; property: "opacity"; value: 1 }
                PropertyAction { target: loaderSelectors; property: "opacity"; value: 1 }
                NumberAnimation { target: finder; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1080; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hideAll"
            SequentialAnimation {
                PropertyAction { target: userInput; property: "opacity"; value: 0 }
                PropertyAction { target: noResultLabel; property: "opacity"; value: 0 }
                PropertyAction { target: userInputLabel; properties: "visible"; value: false }
                PropertyAction { target: buttonsFiltersColumn; property: "opacity"; value: 0 }
                PropertyAction { target: loaderSelectors; property: "source"; value: "" }
                PropertyAction { target: loaderSelectors; property: "active"; value: false }
                NumberAnimation { target: finder; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                PropertyAction { target: root_loader_B; property: "source"; value: "Output.qml" }
                PropertyAction { target: root_loader_A; property: "source"; value: "Network.qml" }
                PropertyAction { target: root; property: "hideFilters_hideFinder_showOutput"; value: false }
            }
        },
        Transition {
            to: "showFilter"
            SequentialAnimation {
                PropertyAction { target: loaderFilter; property: "active"; value: true }
                NumberAnimation { target: finder; easing.amplitude: 1.7; properties: "x"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hideFilter"
            PropertyAction { target: focusDelay; property: "running"; value: true }
            SequentialAnimation {
                NumberAnimation { target: finder; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
            }
        },
        Transition {
            to: "showHelp"
            NumberAnimation { target: finder; easing.amplitude: 1.7; properties: "x"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "hideHelp"
            PropertyAction { target: focusDelay; property: "running"; value: true }
            NumberAnimation { target: finder; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1080; easing.type: Easing.OutQuart }
        }
    ]

    Keys.onPressed: {
        if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
            if (!loaderFilter.active) {
                finder.state = "showHelp"
                screen.state = "showHelp"
                event.accepted = true
            }
        } else if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier)) {
            if (!loaderFilter.active)
                screen.state = "showSetPassword"
            event.accepted = true
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true
        }
    }

    Connections {
        target: cppSignals
        onSignal_no_result: {
            userInput.visible = false
            noResultLabel.visible = true
        }
    }

    Component.onCompleted: {
        root.pornstar = ""
        root.category = ""
        if (root.welcome)
            finder.state = "hideWelcome"
        else
            finder.state = "show"
        focusDelay.start()
    }
}
// Tabs hechos.
