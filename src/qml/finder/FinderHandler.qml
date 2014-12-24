import QtQuick 2.3
import "../global/"

Item {
    id: finderHandler
    x: root.xB

    property bool active
    property bool onCategoryFilter

    Input {
        id: input
        visible: finderHandler.active
        enabled: finderHandler.active
        anchors.centerIn: parent
    }

    Button {
        id: categoryFilter
        width: screen.width/4.06
        height: screen.height/13.67
        label: "CATEGORY"
        labelColor: "white"
        labelSize: screen.height/10
        labelAngle: 64
        labelOriginX: 100
        labelOriginY: 100
        labelAxisX: -0.8
        labelAxisY: 2
        labelAxisZ: -1.5
        opacity: 0
        anchors.left: parent.left
        anchors.leftMargin: 120//parent.width/48.3
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -220
        onClicked: {
            finderHandler.state = "show_filter"
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
        labelAngle: 67
        labelOriginX: 100
        labelOriginY: 100
        labelAxisX: -0.8
        labelAxisY: 2
        labelAxisZ: -1.5
        opacity: 0
        anchors.left: parent.left
        anchors.leftMargin: 190//parent.width/48.3
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -150
        onClicked: {
            finderHandler.state = "show_filter"
            onCategoryFilter = false
        }
    }
    Loader {
        id: filter_loader
        z: 2
        source: "Filters.qml"
        asynchronous: true
        active: false
    }

    Selectors {
        id: selectors
        visible: false
        //opacity: 0 //
        anchors.fill: parent
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
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.3; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: categoryFilter; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: pornstarFilter; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
                }
                PropertyAction { target: selectors; property: "visible"; value: true }
                PropertyAction { target: finderHandler; property: "active"; value: true }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: finderHandler; property: "active"; value: false }
                PropertyAction { target: selectors; property: "visible"; value: false }
                NumberAnimation { duration: 250 }
                ParallelAnimation {
                    NumberAnimation { target: categoryFilter; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: pornstarFilter; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 600; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filter_loader; property: "source"; value: "" }
                PropertyAction { target: screen; property: "state"; value: "show_viewer" }
            }
        },
        Transition {
            to: "show_filter"
            SequentialAnimation {
                PropertyAction { target: finderHandler; property: "active"; value: false }
                PropertyAction { target: filter_loader; property: "active"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: input; easing.amplitude: 1.7; properties: "opacity"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: categoryFilter; easing.amplitude: 1.7; properties: "opacity"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: pornstarFilter; easing.amplitude: 1.7; properties: "opacity"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: selectors; easing.amplitude: 1.7; properties: "opacity"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 96; duration: 2000; easing.type: Easing.OutQuart }
                }
            }
        },
        Transition {
            to: "hide_filter"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: input; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 600; easing.type: Easing.InSine }
                    NumberAnimation { target: categoryFilter; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 600; easing.type: Easing.InSine }
                    NumberAnimation { target: pornstarFilter; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 600; easing.type: Easing.InSine }
                    NumberAnimation { target: selectors; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 600; easing.type: Easing.InSine }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 2100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filter_loader; property: "active"; value: false }
                PropertyAction { target: finderHandler; property: "active"; value: true }
            }
        },
        Transition {
            to: "hide_filter_finder"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 1100; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filter_loader; property: "source"; value: "" }
                PropertyAction { target: root; property: "inputUser"; value: "" }
                PropertyAction { target: screen; property: "state"; value: "show_viewer" }
            }
        }
    ]

    Keys.onPressed: {
        root.init = false
        if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
            if (!filter_loader.active) {
                screen.state = "show_help"
                event.accepted = true
            }
        } else if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier)) {
            if (!filter_loader.active) {
                screen.state = "show_password"
            }
            event.accepted = true
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true
        }
    }

    function set_filter(label) {
        if (onCategoryFilter)
            root.category = label.toUpperCase()
        else
            root.pornstar = label.toUpperCase()
        root.signal_qml_find("", root.pornstar, root.category, "", "", 0, true)
    }

    onFocusChanged: { if (!input.focus) input.forceActiveFocus() }

    Component.onCompleted: { finderHandler.state = "show" }
}
// Tabs hechos.
