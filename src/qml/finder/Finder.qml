import QtQuick 2.4
import QtGraphicalEffects 1.0

import "../global/"

Item {
    id: finder
    enabled: { screen.state !== "show_help" }

    property bool onCategory

    Quality {
        id: quality
        x: parent.width+250 //-parent.width/5.96
        width: parent.width/5.96
        height: parent.height/14.21
        anchors.verticalCenter: input.verticalCenter
        //anchors.bottom: input.top
        //anchors.bottomMargin: parent.height/108
    }

    RectangularGlow {
        id: inputEffect
        color: "black"
        glowRadius: 10
        cornerRadius: 40
        anchors.fill: input
    }
    Input {
        id: input
        x: -parent.width/3.58
        width: parent.width/3.76
        height: parent.height/21.6
        enabled: false
        anchors.verticalCenter: parent.verticalCenter
    }

    Row {
        id: columnFilters
        x: parent.width+10 //-parent.width/4.51
        spacing: parent.width/384
        anchors.verticalCenter: input.verticalCenter
        //anchors.top: input.bottom
        //anchors.topMargin: -parent.height/216
        Button {
            id: btnFilterCategory
            label: "CATEGORY"
            colorIn: "#FA6900"
            onClicked: {
                onCategory = true
                finder.state = "show_filter"
            }
        }
        Button {
            id: btnFilterPornstar
            label: "PORNSTAR"
            colorIn: "#FD2790"
            onClicked: {
                onCategory = false
                finder.state = "show_filter"
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
                ParallelAnimation { //screen.width/18.33 duration: 1200
                    NumberAnimation { target: quality; properties: "x"; to: 950; duration: 1260; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"; to: 520; duration: 1200; easing.type: Easing.InOutQuart }
                } //screen.width/43.63
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: input; property: "enabled"; value: false }
                ParallelAnimation { //-screen.width/5.96
                    NumberAnimation { target: quality; properties: "x"; to: screen.width+10; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/3.58; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"; to: screen.width+10; duration: 600; easing.type: Easing.InOutQuart }
                } //-screen.width/4.51
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
                PropertyAction { target: loader_filter; property: "source"; value: "Filters.qml" }
                ParallelAnimation {
                    NumberAnimation { target: quality; properties: "x"; to: -screen.width/5.96; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/3.58; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"; to: -screen.width/4.51; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                }
            }
        },
        Transition {
            to: "hide_filter"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: quality; properties: "x"; to: screen.width/18.33; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"; to: screen.width/43.63; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"; to: screen.width+10; duration: 1200; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: loader_filter; property: "source"; value: "" }
                PropertyAction { target: input; property: "enabled"; value: true }
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
