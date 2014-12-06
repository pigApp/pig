import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: selectors

    property bool onShowSelectors

    Image {
        id: more
        width: parent.width/45.71
        height: parent.height/60
        sourceSize.width: parent.width/45.71
        sourceSize.height: parent.height/60
        source: "/resources/images/finder/selectors/more.svg"
        anchors.left: parent.left
        anchors.leftMargin: parent.width/42.47
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/28.42
        MouseArea {
            hoverEnabled: true
            onEntered: {
                if (!onShowSelectors && root.full === "" && root.quality === "")
                    selectors.state = "in"
            }
            onHoveredChanged: {
                if (!onShowSelectors && root.full === "" && root.quality === "")
                    selectors.state = "out"
            }
            onClicked: {
                if (!onShowSelectors) {
                    onShowSelectors = true
                    selectors.state = "show_selectors"
                } else {
                    if (root.full !== "" || root.quality !== "")
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
        anchors.left: more.right
        anchors.leftMargin: parent.width/384
        anchors.top: more.bottom
        anchors.topMargin: -screen.height/85
        Image {
            id: fullMovie
            width: screen.width/35.55
            height: screen.height/41.65
            sourceSize.width: screen.width/35.55
            sourceSize.height: screen.height/41.65
            source: {
                if (root.full === "FULL")
                    "/resources/images/finder/selectors/full_ON.svg"
                else
                    "/resources/images/finder/selectors/full.svg"
            }
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
            width: screen.width/35.55
            height: screen.height/41.65
            sourceSize.width: screen.width/35.55
            sourceSize.height: screen.height/41.65
            source: {
                if (root.quality === "SD")
                    "/resources/images/finder/selectors/sd_ON.svg"
                else
                    "/resources/images/finder/selectors/sd.svg"
            }
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
            width: screen.width/35.55
            height: screen.height/20
            sourceSize.width: screen.width/35.55
            sourceSize.height: screen.height/20
            source: {
                if (root.quality === "720p")
                    "/resources/images/finder/selectors/720p_ON.svg"
                else
                    "/resources/images/finder/selectors/720p.svg"
            }
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
            width: screen.width/35.55
            height: screen.height/20
            sourceSize.width: screen.width/35.55
            sourceSize.height: screen.height/20
            source: {
                if (root.quality === "1080p")
                    "/resources/images/finder/selectors/1080p_ON.svg"
                else
                    "/resources/images/finder/selectors/1080p.svg"
            }
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
                NumberAnimation { target: moreShadow; easing.amplitude: 1.7; properties: "opacity"; to: 0; duration: 100; easing.type: Easing.OutQuart }
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
