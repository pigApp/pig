import QtQuick 2.2

Item {
    id: finder

    property string category: ''
    property string pornstar: ''
    property string enableFilter

    TextInput {
        id: input
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.bold: true
        font.capitalization: Font.AllUppercase
        font.pixelSize: 50
        maximumLength: 25
        visible: { if (buttonsFiltersColumn.opacity == 1) true; else false }
        enabled: { if (buttonsFiltersColumn.opacity == 1) true; else false }
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: -150
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -4
        onAccepted: { root.findSIGNAL_QML(input.text, category, pornstar, 0, true) }
        onCursorPositionChanged: { if (noResultLabel.visible) noResultLabel.visible = false; input.visible = true }
        Keys.onPressed: {
            if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier)) {
                if (!loaderFilter.active)
                    screen.state = "showSetPassword"
                event.accepted = true
            } else if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
                if (!loaderFilter.active)
                    screen.state = "showHelp"
                event.accepted = true
            } else if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
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
        font.bold: true
        font.pixelSize: 50
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -4
    }

    Column {
        id: buttonsFiltersColumn
        spacing: 16
        visible: { !root.news && screen.state !== "showSetPassword" && screen.state !== "showHelp" }
        enabled: { !root.news && screen.state !== "showSetPassword" && screen.state !== "showHelp" }
        opacity: 0      
        anchors.left: parent.left
        anchors.leftMargin: 64
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -70
        z: 2
        onEnabledChanged: { if (enabled) input.forceActiveFocus() }
        Button {
            id: categoryFilter
            width: 440
            height: 75
            label: "CATEGORY"
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            labelOutColor: "white"
            onClicked: { 
                finder.state = "showFilter"
                enableFilter = "CATEGORY"
            }
        }
        Button {
            id: pornstarFilter
            width: 445
            height: 75
            label: "PORNSTAR"
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
        if (event.key === Qt.Key_Escape && !(event.modifiers & Qt.ControlModifier)) {
            finder.state = "hideFilter"
            event.accepted = true;
            input.forceActiveFocus()
        } else if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ControlModifier)) {
            root.quitSIGNAL_QML()
        }
    }
    function filtersManager(filter, label) {

        if (filter === 'categoryFilter')
            category = label.toUpperCase()
        else
            pornstar = label.toUpperCase()

        finder.state = "hideAll"
        root.findSIGNAL_QML('', category, pornstar, 0, true)
        noResultLabel.visible = false
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
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: input; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: noResultLabel; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: buttonsFiltersColumn; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                }
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
                PropertyAction { target: input; property: "visible"; value: false }
                PropertyAction { target: input; property: "focus"; value: true }
                PropertyAction { target: noResultLabel; property: "visible"; value: false }
                PropertyAction { target: buttonsFiltersColumn; property: "visible"; value: false }
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
                PropertyAction { target: input; property: "visible"; value: true }
            }
        },
        Transition {
            to: "hideAll"
            SequentialAnimation {
                PropertyAction { target: loaderFilter; property: "source"; value: "" }
                PropertyAction { target: loaderFilter; property: "active"; value: false }
                NumberAnimation { target: filterLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: 0; duration: 1000; easing.type: Easing.OutQuart }
                PropertyAction { target: buttonsFiltersColumn; property: "visible"; value: true }
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
            if (category != '' || pornstar != '')
                finder.state = "hideAll"
            else
                finder.state = "hide"
        }
        onNoResultSIGNAL: {
            input.visible = false
            noResultLabel.visible = true
        }
    }

    Component.onCompleted: {
        if(root.news)
            finder.state = "showNews"
        else
            finder.state = 'show'
        input.forceActiveFocus()
    }
}
// Espacios hechos.
