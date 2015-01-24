import QtQuick 2.4
import "../global/"

Item {
    id: finderHandler

    property bool onCategoryFilter

    Quality {
        id: quality
        x: -parent.width/4.22
        width: parent.width/4.22
        height: parent.height/21.6
        anchors.bottom: filtersColumn.top
        anchors.bottomMargin: parent.height/108
        onXChanged: {
            if (quality.x === (-parent.width/4.22)) {
                if ((root.quality !== "") || (root.full === "1")) {
                    stateQualityIcon.opacity = 1
                    stateQualityIcon.source = "qrc:/img-quality-on"
                } else {
                    stateQualityIcon.opacity = 0.1
                    stateQualityIcon.source = "qrc:/img-quality-in"
                }
            } else {
                stateQualityIcon.opacity = 1
                stateQualityIcon.source = "qrc:/img-quality-out"
            }
        }
    }
    Image {
        id: stateQualityIcon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: 33
        sourceSize.height: 33
        source: "qrc:/img-quality-in"
        opacity: 0.1
        visible: false
        anchors.left: quality.right
        anchors.leftMargin: screen.width/192
        anchors.verticalCenter: quality.verticalCenter
        MouseArea {
            onClicked: {
                if (quality.x === (-screen.width/4.22))
                    finderHandler.state = "show_quality"
                else
                    finderHandler.state = "hide_quality"
            }
            anchors.fill: parent
        }
    }

    Column {
        id: filtersColumn
        x: -parent.width/4.22
        spacing: parent.height/108
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/27
        Button {
            id: categoryFilter
            width: screen.width/4.22
            height: screen.height/21.6
            label: "CATEGORY"
            labelLeftMargin: screen.width/192
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
            label: "PORNSTAR"
            labelLeftMargin: screen.width/192
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
        x: -parent.width/4.22
        width: parent.width/4.22
        height: screen.height/21.6
        enabled: false
        anchors.top: filtersColumn.bottom
        anchors.topMargin: parent.height/108
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
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                }
                ParallelAnimation {
                    NumberAnimation { target: filtersColumn; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                }
                PropertyAction { target: stateQualityIcon; property: "visible"; value: true }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "hide"
            SequentialAnimation {
                PropertyAction { target: input; property: "enabled"; value: false }
                PropertyAction { target: stateQualityIcon; property: "visible"; value: false }
                ParallelAnimation {
                    NumberAnimation { target: quality; properties: "x"; to: -screen.width/4.22; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: filtersColumn; properties: "x"; to: -screen.width/4.22; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/4.22; duration: 300; easing.type: Easing.InOutQuart }
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
                PropertyAction { target: stateQualityIcon; property: "visible"; value: false }
                PropertyAction { target: filter_loader; property: "active"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: quality; properties: "x"; to: -screen.width/4.22; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: filtersColumn; properties: "x"; to: -screen.width/4.22; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: -screen.width/4.22; duration: 300; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 96; duration: 2000; easing.type: Easing.OutQuart }
                }
            }
        },
        Transition {
            to: "hide_filter"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: filtersColumn; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: input; properties: "x"; to: 0; duration: 1200; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: screen.width; duration: 1200; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                }
                PropertyAction { target: filter_loader; property: "active"; value: false }
                PropertyAction { target: stateQualityIcon; property: "visible"; value: true }
                PropertyAction { target: input; property: "enabled"; value: true }
            }
        },
        Transition {
            to: "show_quality"
            ParallelAnimation {
                NumberAnimation { target: stateQualityIcon; properties: "anchors.leftMargin"; to: screen.width/384; duration: 600; easing.type: Easing.InOutQuart }
                NumberAnimation { target: quality; properties: "x"; to: 0; duration: 600; easing.type: Easing.InOutQuart }
            }
        },Transition {
            to: "hide_quality"
            ParallelAnimation {
                NumberAnimation { target: stateQualityIcon; properties: "anchors.leftMargin"; to: screen.width/192; duration: 600; easing.type: Easing.InOutQuart }
                NumberAnimation { target: quality; properties: "x"; to: -screen.width/4.22; duration: 600; easing.type: Easing.InOutQuart }
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
        root.signal_qml_find("", root.pornstar, root.category, root.quality, root.full, 0, true)
    }

    onFocusChanged: { if (!input.focus) input.forceActiveFocus() }

    Component.onCompleted: { finderHandler.state = "show" }
}
// Tabs hechos.
