import QtQuick 2.4
import QtGraphicalEffects 1.0

import "../global/"

Item {
    id: finder

    property bool onCategory

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
        x: parent.width+10
        spacing: parent.width/192
        anchors.verticalCenter: input.verticalCenter
        Button {
            id: btnFilterCategory
            label: "CATEGORY"
            labelColor: "darkgray"
            labelSize: screen.height/20
            labelColorIn: "#FA6900"
            onClicked: {
                onCategory = true
                finder.state = "show_filter"
            }
        }
        Button {
            id: btnFilterPornstar
            label: "PORNSTAR"
            labelColor: "darkgray"
            labelSize: screen.height/20
            labelColorIn: "#FD2790"
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

    Quality {
        id: quality
        x: parent.width+410
        width: parent.width/5.96
        height: parent.height/14.21
        anchors.verticalCenter: input.verticalCenter
    }

    Button {
        id: btnSetting
        x: parent.width+700
        label: "SETTING"
        labelColor: "darkgray"
        labelSize: screen.height/20
        labelColorIn: "black"
        anchors.verticalCenter: input.verticalCenter
        onClicked: {
            if (finder.state !== "show_filter")
                screen.state = "show_setting"
        }
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
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenR"
                        ; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenG"
                        ; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenB"
                        ; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"
                        ; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "blurRadius"
                        ; to: 0; duration: 600; easing.type: Easing.OutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: input; properties: "x"
                        ; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"
                        ; to: screen.width/3.60; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: quality; properties: "x"
                        ; to: screen.width/1.84; duration: 1200; easing.type: Easing.InOutCubic }
                    NumberAnimation { target: btnSetting; properties: "x"
                        ; to: screen.width/1.43; duration: 1400; easing.type: Easing.InOutQuart }
                }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: input; property: "enabled"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: input; properties: "x"
                        ; to: -screen.width/3.58; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"
                        ; to: screen.width+10; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: quality; properties: "x"
                        ; to: screen.width+10; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: btnSetting; properties: "x"
                        ; to: screen.width+10; duration: 430; easing.type: Easing.InOutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"
                        ; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "blurRadius"
                        ; to: 32; duration: 600; easing.type: Easing.OutQuart }
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
                    NumberAnimation { target: input; properties: "x"
                        ; to: -screen.width/1.79; duration: 120; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"
                        ; to: -screen.width/1.79; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: quality; properties: "x"
                        ; to: -screen.width/1.79; duration: 350; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: btnSetting; properties: "x"
                        ; to: screen.width+10; duration: 120; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"
                        ; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                }
            }
        },
        Transition {
            to: "hide_filter"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: input; properties: "x"
                        ; to: 0; duration: 1240; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: columnFilters; properties: "x"
                        ; to: screen.width/3.60; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: quality; properties: "x"
                        ; to: screen.width/1.84; duration: 1100; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: btnSetting; properties: "x"
                        ; to: screen.width/1.43; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"
                        ; to: screen.width+10; duration: 1200; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: loader_filter; property: "source"; value: "" }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "hide_filter_finder"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xa"
                        ; to: screen.width+10; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "blurRadius"
                        ; to: 32; duration: 600; easing.type: Easing.OutQuart }
                }
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"
                    ; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                PropertyAction { target: loader_filter; property: "source"; value: "" }
                PropertyAction { target: root; property: "userInput"; value: "" }
                PropertyAction { target: screen; property: "state"; value: "show_viewer" }
            }
        }
    ]

    Keys.onPressed: {
        root.init = false
        if ((event.key === Qt.Key_H)
            && (event.modifiers & Qt.ControlModifier)) {
            if (finder.state !== "show_filter") {
                screen.state = "show_help"
                event.accepted = true
            }
        } else if ((event.key === Qt.Key_Q)
            && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    onFocusChanged: { if (!input.focus) input.forceActiveFocus() }

    Component.onCompleted: { finder.state = "show" }
}
// Tabs hechos.
