import QtQuick 2.2

Item {
    id: finder

    property string enableFilter

    TextInput {
        id: userInput
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.capitalization: Font.AllUppercase
        font.pixelSize: parent.height/21.6
        maximumLength: 28
        visible: { if (buttonsFiltersColumn.opacity === 1 && !root.welcome ) true; else false }
        enabled: { if (buttonsFiltersColumn.opacity === 1) true; else false }
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: -parent.width/12
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.width/1700
        onAccepted: { root.input = userInput.text; root.findSIGNAL_QML(root.input, root.pornstar, root.category, root.quality, root.full, 0, true) }
        onCursorPositionChanged: { if (noResultLabel.visible) noResultLabel.visible = false; userInput.visible = true }
        Keys.onPressed: {
            if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier) && !root.welcome) {
                if (!loaderFilter.active)
                    screen.state = "showSetPassword"
                event.accepted = true
            } else if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
                if (!loaderFilter.active) {
                    root.welcome = false
                    screen.state = "showHelp"
                    event.accepted = true
                }
            } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                root.quitSIGNAL_QML()
                event.accepted = true
            }
        }
    }
    Text {
        id: noResultLabel
        text: "NO RESULT"
        color: Qt.rgba(0.1, 0.1, 0.1, 0.1)
        font.family: pigFont.name
        font.pixelSize: parent.height/21.6
        visible: false
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.width/1700
    }

    Column {
        id: buttonsFiltersColumn
        spacing: parent.height/67.5
        visible: { !root.news && screen.state !== "showSetPassword" && screen.state !== "showHelp" && !root.welcome }
        enabled: { !root.news && screen.state !== "showSetPassword" && screen.state !== "showHelp" && !root.welcome }
        opacity: 0      
        anchors.left: parent.left
        anchors.leftMargin: parent.width/30
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/15.42
        z: 2
        onEnabledChanged: { if (enabled) userInput.forceActiveFocus() }
        Button {
            id: categoryFilter
            width: screen.width/4.36
            height: screen.height/14.4
            label: "CATEGORY"
            labelSize: screen.height/10.8 // TODO: Falla con resoluciones menores a 1240x720 posiblemente por width y height. Probar logo.height.
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            labelOutColor: "white"
            onClicked: {
                finder.state = "showFilter"
                enableFilter = "CATEGORY"
            }
        }
        Button {
            id: pornstarFilter
            width: screen.width/4.31
            height: screen.height/14.4
            label: "PORNSTAR"
            labelSize: screen.height/10.8 // TODO: Falla con resoluciones menores a 1240x720 posiblemente por width y height.
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            labelOutColor: "white"
            onClicked: {
                finder.state = "showFilter"
                enableFilter = "PORNSTAR"
            }
        }
    }
    Rectangle {
        id: filterLayer
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.right
    }
    Loader {
        id: loaderFilter
        source: "Filters.qml"
        asynchronous: true
        active: false
        anchors.fill: parent
    }
    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            finder.state = "hideFilter"
            event.accepted = true;
            userInput.forceActiveFocus()
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
        }
    }
    function filtersManager(filter, label) {
        if (filter === 'categoryFilter')
            root.category = label.toUpperCase()
        else
            root.pornstar = label.toUpperCase()
        finder.state = "hideAll"
        root.findSIGNAL_QML('', root.pornstar, root.category, root.quality, root.full, 0, true)
        noResultLabel.visible = false
    }
    Loader {
        id: loaderSelectors
        source: "Selectors.qml"
        asynchronous: true
        active: true
        visible: { !root.news && screen.state !== "showSetPassword" && screen.state !== "showHelp" && !root.welcome }
        enabled: { !root.news && screen.state !== "showSetPassword" && screen.state !== "showHelp" && !root.welcome }
        opacity: 0
        anchors.fill: parent
    }

    Text {
        id: welcomeLabel
        text: "WELCOME TO PIG. CTRL H TO HELP." 
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: parent.height/54
        visible: { root.welcome && !root.news && buttonsFiltersColumn.opacity === 1 }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    states: [
        State {
            name: "showNews"
        },
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
            to: "showNews"
            SequentialAnimation {
                NumberAnimation { target: logo; easing.amplitude: 1.65; properties: "opacity"; to: 1.0; duration: 800; easing.type: Easing.OutInElastic }
                ParallelAnimation {
                    NumberAnimation { target: blur; properties: "opacity"; to: 1; duration: 1700; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: blur; properties: "radius"; to: 40; duration: 1700; easing.type: Easing.InOutQuart }
                }
                PropertyAction { target: buttonsFiltersColumn; property: "opacity"; value: 1.0 }
                PropertyAction { target: loaderSelectors; property: "opacity"; value: 1.0 }
                PropertyAction { target: loader; property: "source"; value: "News.qml" }
            }
        },
        Transition {
            to: "show"
            SequentialAnimation {
                NumberAnimation { target: logo; easing.amplitude: 1.65; properties: "opacity"; to: 1.0; duration: 800; easing.type: Easing.OutInElastic }
                ParallelAnimation {
                    NumberAnimation { target: blur; properties: "opacity"; to: 1; duration: 1700; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: blur; properties: "radius"; to: 40; duration: 1700; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 1.0; duration: 400; easing.type: Easing.InOutQuart }
                NumberAnimation { target: loaderSelectors; properties: "opacity"; to: 1.0; duration: 400; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: userInput; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: noResultLabel; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: loaderSelectors; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                }
                PropertyAction { target: loaderSelectors; property: "source"; value: "" }
                PropertyAction { target: loaderSelectors; property: "active"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: blur; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                    NumberAnimation { target: logo; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                }
                PropertyAction { target: loader; property: "source"; value: "Wait.qml" }
                PropertyAction { target: loader_finder_output; property: "source"; value: "Output.qml" }
            }
        },
        Transition {
            to: "showFilter"
            SequentialAnimation {
                PropertyAction { target: userInput; property: "visible"; value: false }
                PropertyAction { target: userInput; property: "focus"; value: true }
                PropertyAction { target: noResultLabel; property: "visible"; value: false }
                PropertyAction { target: buttonsFiltersColumn; property: "visible"; value: false }
                PropertyAction { target: loaderSelectors; property: "visible"; value: false }
                NumberAnimation { target: filterLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                PropertyAction { target: loaderFilter; property: "active"; value: true }
            }
        },
        Transition {
            to: "hideFilter"
            SequentialAnimation {
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                NumberAnimation { target: filterLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: 0; duration: 700; easing.type: Easing.OutQuart }
                PropertyAction { target: buttonsFiltersColumn; property: "visible"; value: true }
                PropertyAction { target: loaderSelectors; property: "visible"; value: true }
                PropertyAction { target: userInput; property: "visible"; value: true }
            }
        },
        Transition {
            to: "hideAll"
            SequentialAnimation {
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                NumberAnimation { target: filterLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: 0; duration: 1000; easing.type: Easing.OutQuart }
                PropertyAction { target: buttonsFiltersColumn; property: "visible"; value: true }
                PropertyAction { target: loaderSelectors; property: "source"; value: "" }
                PropertyAction { target: loaderSelectors; property: "active"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: blur; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                    NumberAnimation { target: logo; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                }
                PropertyAction { target: loader; property: "source"; value: "Wait.qml" }
                PropertyAction { target: loader_finder_output; property: "source"; value: "Output.qml" }
            }
        }
    ]

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
        if(root.news)
            finder.state = "showNews"
        else
            finder.state = 'show'
        root.input = ""
        root.pornstar = ""
        root.category = ""
        userInput.forceActiveFocus()
    }
}
// Espacios hechos.
