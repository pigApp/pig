import QtQuick 2.4

Item {
    id: selectors

    property bool onShowSelectors

    Image {
        id: moreIcon
        width: screen.width/58.18
        height: screen.height/32.72
        sourceSize.width: 33
        sourceSize.height: 33
        source: "qrc:/img-more"
        opacity: 0.5
        anchors.left: parent.left
        anchors.leftMargin: screen.width/320
        anchors.verticalCenter: parent.verticalCenter
        MouseArea {
            hoverEnabled: true
            onEntered: { if (!onShowSelectors && root.full === "" && root.quality === "") selectors.state = "in" }
            onHoveredChanged: { if (!onShowSelectors && root.full === "" && root.quality === "") selectors.state = "out" }
            onClicked: {
                if (!onShowSelectors) {
                    finderHandler.state = "show_selectors"
                    onShowSelectors = true
                    statesDelay.restart()
                } else {
                    statesDelay.stop()
                    finderHandler.state = "hide_selectors"
                    if (root.full !== "" || root.quality !== "")
                        selectors.state = "hide_active_selectors"
                    else
                        selectors.state = "hide_selectors"
                    onShowSelectors = false
                }
            }
            anchors.fill: parent
        }
    }

    Row {
        id: selectorsRow
        spacing: screen.width/384
        opacity: 0
        anchors.left: moreIcon.right
        anchors.leftMargin: screen.width/160
        anchors.verticalCenter: moreIcon.verticalCenter
        Image {
            id: fullMovieIcon
            width: screen.width/29.53
            height: screen.height/16.61
            sourceSize.width: 65
            sourceSize.height: 65
            source: { if (root.full === "FULL") "qrc:/img-full"; else "qrc:/img-full-off" }
            MouseArea {
                onClicked: { if (root.full === "FULL") root.full = ""; else root.full= "FULL" }
                anchors.fill: parent
            }
        }
        Image {
            id: sdIcon
            width: screen.width/29.53
            height: screen.height/16.61
            sourceSize.width: 65
            sourceSize.height: 65
            source: { if (root.quality === "SD") "qrc:/img-480p"; else "qrc:/img-480p-off" }
            MouseArea {
                onClicked: { if (root.quality === "SD") root.quality = ""; else root.quality = "SD" }
                anchors.fill: parent
            }
        }
        Image {
            id: hdIcon
            width: screen.width/29.53
            height: screen.height/16.61
            sourceSize.width: 65
            sourceSize.height: 65
            source: { if (root.quality === "720p") "qrc:/img-720p"; else "qrc:/img-720p-off" }
            MouseArea {
                onClicked: { if (root.quality === "720p") root.quality = ""; else root.quality = "720p" }
                anchors.fill: parent
            }
        }
        Image {
            id: fullHdIcon
            width: screen.width/29.53
            height: screen.height/16.61
            sourceSize.width: 65
            sourceSize.height: 65
            source: { if (root.quality === "1080p") "qrc:/img-1080p"; else "qrc:/img-1080p-off" }
            MouseArea {
                onClicked: { if (root.quality === "1080p") root.quality = ""; else root.quality = "1080p" }
                anchors.fill: parent
            }
        }
    }

    Timer {
        id: statesDelay
        running: false
        repeat: false
        interval: 300
        onTriggered: { selectors.state = "show_selectors" }
    }

    states: [
        State {
            name: "in"
        },
        State {
            name: "out"
        },
        State {
            name: "show_selectors"
        },
        State {
            name: "hide_selectors"
        },
        State {
            name: "hide_active_selectors"
        }
    ]
    transitions: [
        Transition {
            to: "in"
            NumberAnimation { target: moreIcon; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 100; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "out"
            NumberAnimation { target: moreIcon; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "show_selectors"
            SequentialAnimation {
                NumberAnimation { target: moreIcon; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 100; easing.type: Easing.OutQuart }
                NumberAnimation { target: selectorsRow; properties: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "hide_selectors"
            SequentialAnimation {
                NumberAnimation { target: selectorsRow; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.InOutQuart }
                NumberAnimation { target: moreIcon; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
                PropertyAction { target: selectors; property: "onShowSelectors"; value: false }
            }
        },
        Transition {
            to: "hide_active_selectors"
            SequentialAnimation {
                NumberAnimation { target: selectorsRow; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                PropertyAction { target: selectors; property: "onShowSelectors"; value: false }
            }
        }
    ]

    Component.onCompleted: {
        if (root.full === "FULL" || root.quality !== "") {
            onShowSelectors = true
            selectors.state = "show_selectors"
        }
    }
}
// Tabs hechos.
