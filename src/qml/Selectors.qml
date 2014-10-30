import QtQuick 2.2

Item {
    id: selectors

    Text {
        id: selectorsLabel
        text: "FILTERS"
        color: Qt.rgba(0.1, 0.1, 0.1, 1)
        font.family: pigFont.name
        font.bold: true
        font.pixelSize: screen.height/54
        anchors.left: parent.left
        anchors.leftMargin: parent.width/22.27
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/30.85
        MouseArea {
            id: selectorsMousearea
            onClicked: {
                if (!root.onShowSelectors) {
                    root.onShowSelectors = true
                    selectors.state = "onShowSelectors"
                } else {
                    root.onShowSelectors = false
                    if (root.full !== "" || root.quality !== "")
                        selectors.state = "hideActiveSelectors"
                    else
                        selectors.state = "hideSelectors"
                }
            }
            anchors.fill: parent
        }
    }

    Row {
        id: selectorsRow
        spacing: parent.width/384
        opacity: 0
        anchors.left: parent.left
        anchors.leftMargin: parent.width/22.27
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/19.63

        Text {
            id: fullMovieLabel
            text: "FULL"
            color: { if (root.full === "FULL") "red"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/54
            MouseArea {
                id: fullMovieMousearea
                onClicked: {
                    if (root.full === "FULL")
                        root.full = ""
                    else
                        root.full= "FULL"
                }
                anchors.fill: parent
            }
        }
        Text {
            id: standardLabel
            text: " SD"
            color: { if (root.quality === "SD") "red"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/54
            MouseArea {
                id: standardMousearea
                onClicked: {
                    if (root.quality === "SD")
                        root.quality = ""
                    else
                        root.quality = "SD"
                }
                anchors.fill: parent
            }
        }
        Text {
            id: mediumLabel
            text: "720p"
            color: { if (root.quality === "720p") "red"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/54
            MouseArea {
                id: mediumMousearea
                onClicked: {
                    if (root.quality === "720p")
                        root.quality = ""
                    else
                        root.quality = "720p"
                }
                anchors.fill: parent
            }
        }
        Text {
            id: highLabel
            text: "1080p"
            color: { if (root.quality === "1080p") "red"; else Qt.rgba(0.1, 0.1, 0.1, 1) }
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: screen.height/54
            MouseArea {
                id: highMousearea
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
            name: "onShowSelectors"
        },
        State {
            name: "hideSelectors"
        },
        State {
            name: "hideActiveSelectors"
        }
    ]
    transitions: [
        Transition {
            to: "onShowSelectors"
            PropertyAction { target: selectorsLabel; property: "color"; value: "red" }
            NumberAnimation { target: selectorsRow; properties: "opacity"; to: 1.0; duration: 200; easing.type: Easing.InOutQuart }
        },
        Transition {
            to: "hideSelectors"
            SequentialAnimation {
                NumberAnimation { target: selectorsRow; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                PropertyAction { target: selectorsLabel; property: "color"; value: Qt.rgba(0.1, 0.1, 0.1, 1) }
            }
        },
        Transition {
            to: "hideActiveSelectors"
            NumberAnimation { target: selectorsRow; properties: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
        }
    ]

    Component.onCompleted: {
        if (root.onShowSelectors)
            selectors.state = "onShowSelectors"
    }
}
