import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    id: finder

    property string category: ''
    property string pornstar: ''
    property string enableFilter

    signal showF(variant filter)
    signal hideF()

    TextInput {
        id: input
        color: Qt.rgba(0.1, 0.1, 0.1, 0.2)
        font.family: pigFont.name
        font.bold: true
        font.capitalization: Font.AllUppercase
        font.pixelSize: 55
        maximumLength: 25
        visible: { buttonsFindColumn.visible && buttonsFindColumn.opacity == 1 }
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: -150
        anchors.verticalCenter: parent.verticalCenter
        onAccepted: {
            if (!loaderFilters.active && !loaderSetPassword.active) {
                if (noResultLabel.visible) { noResultLabel.visible = false }
                root.findDb(input.text, category, pornstar, 0, true)
            }
        }
        onCursorPositionChanged: { if (noResultLabel.visible) noResultLabel.visible = false; input.visible = true }
        Keys.onPressed: {
            if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier)
                && !root.okPass && !root.failPass) {
                if (finder.state == 'showFinder' || finder.state == 'hideFilter') {
                    buttonsFindColumn.visible = false
                    input.visible = false
                    noResultLabel.visible = false
                    loaderSetPassword.active = true
                }
                event.accepted = true;
            }
            if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ShiftModifier)) {
                root.quit()
                event.accepted = true;
            }
        }
    }
    Text {
        id: noResultLabel
        text: "NO RESULT"
        color: Qt.rgba(0.1, 0.1, 0.1, 0.1)
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: 55
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Column {
        id: buttonsFindColumn
        spacing: 15
        opacity: 0
        anchors.left: parent.left
        anchors.leftMargin: 40
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -70
        z: 2
        Button {
            id: categoryFind
            width: 480
            height: 75
            label: "CATEGORY"
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            labelOutColor: "white"
            onClicked: finder.showF(categoryFind)
        }
        Button {
            id: pornstarFind
            width: 485
            height: 75
            label: "PORNSTAR"
            labelInColor: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            labelOutColor: "white"
            onClicked: finder.showF(pornstarFind)
        }
    }
    onShowF: {
        finder.state = "showFilter"
        enableFilter = filter.label
        bypassFilter.start()
    }
    onHideF: {
        finder.state = "hideFilter"
        loaderFilters.active = false
    }
    Timer {
        id: bypassFilter
        running: false
        repeat: false
        interval: 1150
        onTriggered: loaderFilters.active = true
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
        id: loaderFilters
        source: "Filters.qml"
        asynchronous: true
        active: false
        anchors.fill: parent
    }
    function filtersManager(filter, label) {

        if (filter === 'categoryFind')
            category = label.toUpperCase()
        else
            pornstar = label.toUpperCase()

        finder.state = "hideFilter_hideFinder"
        root.findDb('', category, pornstar, 0, true)
        noResultLabel.visible = false
    }

    Loader {
        id: loaderSetPassword
        source: "SetPassword.qml"
        active: false
        anchors.fill: parent
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape && !(event.modifiers & Qt.ShiftModifier)) {  
            if (loaderFilters.active) {
                finder.hideF()
            } else {
                loaderSetPassword.active = false
                buttonsFindColumn.visible = true
                input.visible = false
                noResultLabel.visible = false
                input.forceActiveFocus()
            }
            event.accepted = true;
        } else if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ShiftModifier)) {  
            root.quit()
            event.accepted = true;
        }
    }

    Loader {
        id: loaderNews
        source: "qrc:/src/qml/News.qml"
        active: root.news
        asynchronous: true
        visible: status == Loader.Ready
        anchors.fill: parent
    }

    states: [
        State {
            name: "showFinder"
        },
        State {
            name: "hideFinde"
        },
        State {
            name: "showFilter"
        },
        State {
            name: "hideFilter"
        },
        State {
            name: "hideFilter_hideFinder"
        }
    ]
    transitions: [
        Transition {
            to: "showFinder"
            SequentialAnimation {
                NumberAnimation { target: pigLogo; easing.amplitude: 1.65; properties: "opacity"; to: 1.0; duration: 800; easing.type: Easing.OutInElastic }
                ParallelAnimation {
                    NumberAnimation { target: blur; properties: "opacity"; to: 1; duration: 1700; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: blur; properties: "radius"; to: 40; duration: 1700; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 1.0; duration: 400; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "hideFinder"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: noResultLabel; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: loaderNews; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: blur; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                    NumberAnimation { target: pigLogo; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                }
                PropertyAction { target: loaderWaitMsg; property: "active"; value: true }
                PropertyAction { target: loaderOutput; property: "source"; value: "qrc:/src/qml/Output.qml" }
                PropertyAction { target: loaderOutput; property: "active"; value: true } 
            }
        },
        Transition {
            to: "showFilter"
            SequentialAnimation {
                PropertyAction { target: input; property: "visible"; value: false }
                PropertyAction { target: noResultLabel; property: "visible"; value: false }
                PropertyAction { target: loaderNews; property: "visible"; value: false }
                PropertyAction { target: input; property: "focus"; value: true }
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 0; duration: 50; easing.type: Easing.InOutQuart }
                PropertyAction { target: buttonsFindColumn; property: "visible"; value: false }
                NumberAnimation { target: filterLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hideFilter"
            SequentialAnimation {
                NumberAnimation { target: filterLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: 0; duration: 700; easing.type: Easing.OutQuart }
                PropertyAction { target: buttonsFindColumn; property: "visible"; value: true }
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 1; duration: 50; easing.type: Easing.InOutQuart }
                PropertyAction { target: input; property: "visible"; value: true }
                PropertyAction { target: loaderNews; property: "visible"; value: true }
            }
        },
        Transition {
            to: "hideFilter_hideFinder"
            SequentialAnimation {
                PropertyAction { target: loaderFilters; property: "visible"; value: false }
                PropertyAction { target: loaderFilters; property: "source"; value: "" }
                NumberAnimation { target: filterLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: 0; duration: 1000; easing.type: Easing.OutQuart }
                PropertyAction { target: buttonsFindColumn; property: "visible"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 0; duration: 50; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: loaderNews; properties: "opacity"; to: 0; duration: 50; easing.type: Easing.InOutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: blur; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                    NumberAnimation { target: pigLogo; easing.amplitude: 1.65; properties: "opacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                }
                PropertyAction { target: loaderWaitMsg; property: "active"; value: true }
                PropertyAction { target: loaderOutput; property: "source"; value: "qrc:/src/qml/Output.qml" }
                PropertyAction { target: loaderOutput; property: "active"; value: true }
            }
        }
    ]

    Connections {
        target: cppSignals
        
        onShowOutputSIGNAL:{
            if (category != '' || pornstar != '')
                finder.state = "hideFilter_hideFinder"
            else
                finder.state = "hideFinder"
        }
        onNoResultSIGNAL: {
            input.visible = false
            noResultLabel.visible = true
        }
    }

    Component.onCompleted: { finder.state = 'showFinder'; input.forceActiveFocus() }
}
// Espacios hechos.
