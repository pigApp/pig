import QtQuick 2.3

Item {
    id: finder

    property bool news: root.news
    property string activeFilter

    Column {
        id: welcomeRow
        spacing: -parent.height/36
        visible: { root.welcome && !root.news && buttonsFiltersColumn.opacity === 1 }
        anchors.centerIn: parent
        Text {
            id: welcomeLabel
            text: "WELCOME TO PIG"
            color: "black"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/10
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id: welcomeHelpLabel
            text: "CTRL H TO HELP"
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/23
            anchors.horizontalCenter: welcomeLabel.horizontalCenter
        }
    }

    Rectangle {
        id: topRedLine
        x: -parent.width/58.18
        width: parent.width/58.18
        height: 2
        color: "red"
        visible: { !root.news && !root.welcome && screen.state !== "showSetPassword" }
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/46.95
    }
    Rectangle {
        id: centerRedLine
        x: parent.width
        width: parent.width/1.4
        height: 2
        color: "red"
        visible: { !root.news && !root.welcome && screen.state !== "showSetPassword" }
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/46.95
    }

    Text {
        id: userInputLabel
        text: "TYPE TO FIND"
        color: "red"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        opacity: 0
        anchors.centerIn: parent
        anchors.verticalCenterOffset: parent.height/120
    }
    TextInput {
        id: userInput
        color: "red"
        font.family: pigFont.name
        font.bold: true
        font.capitalization: Font.AllUppercase
        font.pixelSize: screen.height/23
        visible: { !root.news && !root.welcome && loaderSelectors.opacity === 1.0 && screen.state !== "showSetPassword" }
        enabled: { !root.news && !root.welcome && loaderSelectors.opacity === 1.0 && screen.state !== "showSetPassword" }
        maximumLength: 21
        cursorVisible: false
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -parent.width/112
        onAccepted: {
            root.input = userInput.text
            root.findSIGNAL_QML(root.input, root.pornstar, root.category, root.quality, root.full, 0, true)
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
        color: "red"
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/23
        visible: false
        anchors.centerIn: parent
    }

    Column {
        id: buttonsFiltersColumn
        spacing: parent.height/54
        visible: { !root.news && !root.welcome && screen.state !== "showSetPassword" }
        enabled: { !root.news && !root.welcome && screen.state !== "showSetPassword" }
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
        root.findSIGNAL_QML('', root.pornstar, root.category, root.quality, root.full, 0, true)
    }

    Loader {
        id: loaderSelectors
        source: "Selectors.qml"
        asynchronous: true
        active: true
        visible: { !root.news && !root.welcome && screen.state !== "showSetPassword" }
        enabled: { !root.news && !root.welcome && screen.state !== "showSetPassword" }
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
            name: "showFilter"
        },
        State {
            name: "hideFilter"
        },
        State {
            name: "hideAll"
        }
    ]
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: topRedLine; properties: "x"; to: 0; duration: 500; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: centerRedLine; properties: "x"; to: screen.width/3.1; duration: 500; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 1.0; duration: 60; easing.type: Easing.InOutQuart }
                ParallelAnimation {
                    NumberAnimation { target: loaderSelectors; properties: "opacity"; to: 1.0; duration: 80; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: userInputLabel; properties: "opacity"; to: 1.0; duration: 200; easing.type: Easing.InOutQuart }
                    PropertyAction { target: userInput; property: "text"; value: root.input }
                }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: userInput; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: loaderSelectors; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: userInputLabel; properties: "opacity"; to: 0; duration: 80; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 0; duration: 60; easing.type: Easing.InOutQuart }
                ParallelAnimation {
                    NumberAnimation { target: topRedLine; properties: "x"; to: -screen.width/58.18; duration: 400; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: centerRedLine; properties: "x"; to: screen.width; duration: 400; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: centerRedLine; properties: "x"; to: screen.width; duration: 100}
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                PropertyAction { target: loaderSelectors; property: "source"; value: "" }
                PropertyAction { target: loaderSelectors; property: "active"; value: false }
                PropertyAction { target: loader_finder_output; property: "source"; value: "Output.qml" }
                PropertyAction { target: loader; property: "source"; value: "Wait.qml" }
            }
        },
        Transition {
            to: "showFilter"
            SequentialAnimation {
                PropertyAction { target: loaderFilter; property: "active"; value: true }
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hideFilter"
            SequentialAnimation {
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                PropertyAction { target: userInput; property: "focus"; value: true }
            }
        },
        Transition {
            to: "hideAll"
            SequentialAnimation {
                PropertyAction { target: userInput; property: "opacity"; value: 0 }
                PropertyAction { target: noResultLabel; property: "opacity"; value: 0 }
                NumberAnimation { target: userInputLabel; properties: "opacity"; to: 0 }
                PropertyAction { target: buttonsFiltersColumn; property: "opacity"; value: 0 }
                PropertyAction { target: topRedLine; property: "opacity"; value: 0 }
                PropertyAction { target: centerRedLine; property: "opacity"; value: 0 }
                PropertyAction { target: loaderSelectors; property: "source"; value: "" }
                PropertyAction { target: loaderSelectors; property: "active"; value: false }
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                PropertyAction { target: loader_finder_output; property: "source"; value: "Output.qml" }
                PropertyAction { target: loader; property: "source"; value: "Wait.qml" }
            }
        }
    ]

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape && finder.state === "showFilter") {
            finder.state = "hideFilter"
            event.accepted = true
        } else if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
            if (!loaderFilter.active) {
                screen.state = "showHelp"
                event.accepted = true
            }
        } else if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier) && !root.welcome) {
            if (!loaderFilter.active)
                screen.state = "showSetPassword"
            event.accepted = true
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
            event.accepted = true
        }
    }

    onNewsChanged: { if (!news) finder.state = "show" }

    Connections {
        target: cppSignals
        onShowOutputSIGNAL:{
            if (root.pornstar !== "" || root.category !== "")
                finder.state = "hideAll"
            else
                finder.state = "hide"
        }
        onNoResultSIGNAL: {
            userInput.visible = false
            noResultLabel.visible = true
        }
    }

    Component.onCompleted: {
        root.pornstar = ""
        root.category = ""

        if(news)
            screen.state = "showNews"
        else
            finder.state = 'show'

        if (root.welcome)
            finder.forceActiveFocus()
        else
            userInput.forceActiveFocus()
    }
}
// Espacios hechos.
