import QtQuick 2.4
import QtGraphicalEffects 1.0

import "../global/"

Item {
    id: finder
    enabled: { screen.state !== "show_help" }

    property bool onCategory

    Quality {
        id: quality
        x: -parent.width/3.17
        width: parent.width/3.17
        height: parent.height/14.4
        anchors.bottom: columnFilters.top
        anchors.bottomMargin: -parent.height/54
        onXChanged: {
            if (x === (-parent.width/3.17)) {
                if ((root.quality !== "") || (root.full === "1"))
                    iconQuality.source = "qrc:/img-quality-on"
                else
                    iconQuality.source = "qrc:/img-quality-in"
            } else {
                iconQuality.source = "qrc:/img-quality-out"
            }
        }
    }
    Image {
        id: iconQuality
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: width
        sourceSize.height: height
        source: "qrc:/img-quality-in"
        visible: false
        anchors.left: quality.right
        anchors.verticalCenter: quality.verticalCenter
        anchors.verticalCenterOffset: parent.height/540
        MouseArea {
            onClicked: {
                if (quality.x === (-screen.width/3.17))
                    finder.state = "show_quality"
                else
                    finder.state = "hide_quality"
            }
            anchors.fill: parent
        }
    }

    Column {
        id: columnFilters
        x: -parent.width/3.87
        spacing: -parent.height/21.6
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/27
        Button {
            id: btnFilterCategory
            label: "CATEGORY"
            labelBold: true
            labelMargin: screen.width/384
            onClicked: {
                finder.state = "show_filter"
                onCategory = true
            }
        }
        Button {
            id: btnFilterPornstar
            label: "PORNSTAR"
            labelBold: true
            labelMargin: screen.width/384
            onClicked: {
                finder.state = "show_filter"
                onCategory = false
            }
        }
    }
    Loader {
        id: loader_filter
        z: 2
        source: ""
        asynchronous: true
        visible: { status === Loader.Ready }
    }

    RectangularGlow {
        id: inputEffect
        color: "black"
        glowRadius: 2
        cornerRadius: 40
        anchors.fill: input
    }
    Input {
        id: input
        x: -parent.width/3.58
        width: parent.width/3.76
        height: screen.height/21.6
        enabled: false
        anchors.top: columnFilters.bottom
        anchors.topMargin: -parent.height/360
    }

    Welcome {
        id: welcome
        visible: root.init
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/154.28
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
            name: "show_quality"
        },
        State {
            name: "hide_quality"
        },
        State {
            name: "hide_filter_finder"
        }
    ]
    transitions: [
        Transition {
            to: "show"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenR"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenG"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenB"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: columnFilters; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                }
                PropertyAction { target: iconQuality; property: "visible"; value: true }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: input; property: "enabled"; value: false }
                PropertyAction { target: iconQuality; property: "visible"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: quality; properties: "x"; to: -screen.width/3.17; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"; to: -screen.width/3.87; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/3.58; duration: 300; easing.type: Easing.InOutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 600; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: screen; property: "state"; value: "show_viewer" }
            }
        },
        Transition {
            to: "show_filter"
            SequentialAnimation {
                PropertyAction { target: input; property: "enabled"; value: false }
                PropertyAction { target: iconQuality; property: "visible"; value: false }
                PropertyAction { target: loader_filter; property: "source"; value: "Filters.qml" }
                ParallelAnimation {
                    NumberAnimation { target: quality; properties: "x"; to: -screen.width/3.17; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"; to: -screen.width/3.87; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/3.58; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                }
            }
        },
        Transition {
            to: "hide_filter"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: columnFilters; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: screen.width+10; duration: 1200; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: loader_filter; property: "source"; value: "" }
                PropertyAction { target: iconQuality; property: "visible"; value: true }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "show_quality"
            ParallelAnimation {
                NumberAnimation { target: iconQuality; properties: "anchors.leftMargin"; to: -screen.width/128; duration: 600; easing.type: Easing.InOutQuart }
                NumberAnimation { target: quality; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
            }
        },Transition {
            to: "hide_quality"
            ParallelAnimation {
                NumberAnimation { target: iconQuality; properties: "anchors.leftMargin"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                NumberAnimation { target: quality; properties: "x"; to: -screen.width/3.17; duration: 600; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "hide_filter_finder"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: screen.width+10; duration: 1200; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 1200; easing.type: Easing.OutQuart }
                }
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                PropertyAction { target: loader_filter; property: "source"; value: "" }
                PropertyAction { target: root; property: "userInput"; value: "" }
                PropertyAction { target: screen; property: "state"; value: "show_viewer" }
            }
        }
    ]

    Keys.onPressed: {
        root.init = false
        if ((event.key === Qt.Key_S) && (event.modifiers & Qt.ControlModifier)) {
            if (finder.state !== "show_filter")
                screen.state = "show_setting"
            event.accepted = true
        } else if ((event.key === Qt.Key_H) && (event.modifiers & Qt.ControlModifier)) {
            if (finder.state !== "show_filter") {
                root.help_finder = true
                screen.state = "show_help"
                event.accepted = true
            }
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    onFocusChanged: { if (!input.focus) input.forceActiveFocus() }

    Component.onCompleted: { finder.state = "show" }
}
// Tabs hechos.
