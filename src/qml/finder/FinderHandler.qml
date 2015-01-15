import QtQuick 2.4
import "../global/"

Item {
    id: finderHandler

    property bool onCategoryFilter

    Column {
        id: filtersButtonsColumn
        x: -screen.width/3.84
        spacing: parent.height/108
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/27
        onXChanged: { if (finderHandler.state === "show" && selectors.onShowSelectors) filtersButtonsColumn.anchors.verticalCenterOffset = -screen.height/17.41 }
        Button {
            id: categoryFilter
            width: screen.width/4.22
            height: screen.height/21.6
            label: " CATEGORY"
            labelColor: "white"
            labelFont: customFont.name
            labelLetterSpacing: screen.width/960
            labelSize: screen.height/23
            gridLayerWidth: screen.width/4.22
            gridLayerHeight: screen.height/21.6
            gridLayerSourceSizeWidth: 455
            gridLayerSourceSizeHeight: 50
            gridLayerVisible: true
            onClicked: {
                finderHandler.state = "show_filter"
                onCategoryFilter = true
            }
        }
        Button {
            id: pornstarFilter
            width: screen.width/4.22
            height: screen.height/21.6
            label: " PORNSTAR"
            labelColor: "white"
            labelFont: customFont.name
            labelLetterSpacing: screen.width/960
            labelSize: screen.height/23
            gridLayerWidth: screen.width/4.22
            gridLayerHeight: screen.height/21.6
            gridLayerSourceSizeWidth: 455
            gridLayerSourceSizeHeight: 50
            gridLayerVisible: true
            onClicked: {
                finderHandler.state = "show_filter"
                onCategoryFilter = false
            }
        }
    }
    Loader {
        id: filter_loader
        z: 2
        source: "Filters.qml"
        asynchronous: true
        active: false
    }

    Input {
        id: input
        x: -screen.width/3.84
        enabled: false
        anchors.top: filtersButtonsColumn.bottom
        anchors.topMargin: parent.height/108
    }

    Selectors {
        id: selectors
        visible: false
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/10.8
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
            name: "show_selectors"
        },
        State {
            name: "hide_selectors"
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
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: filtersButtonsColumn; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                }
                PropertyAction { target: selectors; property: "visible"; value: true }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: input; property: "enabled"; value: false }
                PropertyAction { target: selectors; property: "visible"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: filtersButtonsColumn; properties: "x"; to: -screen.width/3.84; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/3.84; duration: 300; easing.type: Easing.InOutQuart }
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
                PropertyAction { target: input; property: "enabled"; value: false }
                PropertyAction { target: selectors; property: "visible"; value: false }
                PropertyAction { target: filter_loader; property: "active"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: filtersButtonsColumn; properties: "x"; to: -screen.width/3.84; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/3.84; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 96; duration: 2000; easing.type: Easing.OutQuart }
                }
            }
        },
        Transition {
            to: "hide_filter"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: filtersButtonsColumn; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: screen.width; duration: 1200; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filter_loader; property: "active"; value: false }
                PropertyAction { target: selectors; property: "visible"; value: true }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "show_selectors"
            NumberAnimation { target: filtersButtonsColumn; easing.amplitude: 1.7; properties: "anchors.verticalCenterOffset"; to: -screen.height/17.41; duration: 600; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "hide_selectors"
            SequentialAnimation {
                NumberAnimation { duration: 300 }
                NumberAnimation { target: filtersButtonsColumn; easing.amplitude: 1.7; properties: "anchors.verticalCenterOffset"; to: -screen.height/27; duration: 600; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hide_filter_finder"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: screen.width; duration: 1200; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 1200; easing.type: Easing.OutQuart }
                }
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
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
