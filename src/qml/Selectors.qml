import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: selectors

    property bool onShowSelectors

    Image {
        id: more
        width: 56 // TODO: ver el tamaño y pasarlo a parent.width/...
        height: 56
        sourceSize.width: 56
        sourceSize.height: 56
        source: "/resources/images/finder/selectors/more.svg"
        anchors.left: parent.left
        anchors.leftMargin: parent.width/40.27
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/23.85
        MouseArea {
            hoverEnabled: true
            onEntered: {
                if (!onShowSelectors)
                    selectors.state = "in"
            }
            onHoveredChanged: {
                if (!onShowSelectors)
                    selectors.state = "out"
            }
            onClicked: {
                if (!onShowSelectors) {
                    onShowSelectors = true
                    selectors.state = "show_selectors"
                } else {
                    onShowSelectors = false
                    if (root.full === "FULL" || root.quality !== "")
                        selectors.state = "hide_active_selectors"
                    else
                        selectors.state = "hide_selectors"
                }
            }
            anchors.fill: parent
        }
    }
    DropShadow {
        id: moreShadow
        color: "white"
        source: more
        radius: 8
        samples: 32
        opacity: 0
        anchors.fill: more
    }

    Row {
        id: selectorsRow
        spacing: parent.width/384
        opacity: 0
        anchors.left: parent.left
        anchors.leftMargin: parent.width/40.27
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 93//parent.height/14.63
        Image {
            id: fullMovie
            width: 64
            height: 64
            sourceSize.width: 64
            sourceSize.height: 64
            source: "/resources/images/finder/selectors/sd.svg"
            opacity: { if (root.full === "FULL") 1; else 0.7 }
            MouseArea {
                onClicked: {
                    if (root.full === "FULL")
                        root.full = ""
                    else
                        root.full= "FULL"
                }
                anchors.fill: parent
            }
        }
        Image {
            id: standard
            width: 64
            height: 64
            sourceSize.width: 64
            sourceSize.height: 64
            source: "/resources/images/finder/selectors/sd.svg"
            opacity: { if (root.quality === "SD") 1; else 0.7 }
            MouseArea {
                onClicked: {
                    if (root.quality === "SD")
                        root.quality = ""
                    else
                        root.quality = "SD"
                }
                anchors.fill: parent
            }
        }
        Image {
            id: hd
            width: 64
            height: 64
            sourceSize.width: 64
            sourceSize.height: 64
            source: "/resources/images/finder/selectors/1080.svg"
            opacity: { if (root.quality === "720p") 1; else 0.7 }
            MouseArea {
                onClicked: {
                    if (root.quality === "720p")
                        root.quality = ""
                    else
                        root.quality = "720p"
                }
                anchors.fill: parent
            }
        }
        Image {
            id: fullHd
            width: 64
            height: 64
            sourceSize.width: 64
            sourceSize.height: 64
            source: "/resources/images/finder/selectors/1080.svg"
            opacity: { if (root.quality === "1080p") 1; else 0.7 }
            MouseArea {
                onClicked: {
                    if (root.quality === "1080p")
                        root.quality = ""
                    else
                        root.quality = "1080p"
                }
                anchors.fill: parent
            }
        }
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
            NumberAnimation { target: moreShadow; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "out"
            NumberAnimation { target: moreShadow; easing.amplitude: 1.7; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.OutQuart }
        },
        Transition {
            to: "show_selectors"
            SequentialAnimation {
                NumberAnimation { target: moreShadow; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
                NumberAnimation { target: selectorsRow; properties: "opacity"; to: 1.0; duration: 200; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "hide_selectors"
            SequentialAnimation {
                NumberAnimation { target: selectorsRow; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                NumberAnimation { target: moreShadow; easing.amplitude: 1.7; properties: "opacity"; to: 0.5; duration: 100; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hide_active_selectors"
            NumberAnimation { target: selectorsRow; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
        }
    ]

    Component.onCompleted: {
        if (root.full === "FULL" || root.quality !== "") {
            onShowSelectors = true
            selectors.state = "show_selectors"
        }
    }
}
