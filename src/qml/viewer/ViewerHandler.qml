import QtQuick 2.3

Item {
    id: viewerHandler
    x: root.xB

    property int location_block: 0
    property int location_global
    property int offset: 0
    property int current_film: 1
    property var coverStatus: []

    ListModel { id: model }

    PathView {
        id: view
        model: model
        delegate: delegate
        enabled: false
        anchors.fill: parent

        property int counter: 5

        path: Path {
            startX: screen.width/2.4
            startY: screen.height/2
            PathAttribute {
                name: "recipe_z"
                value: 2
            }
            PathAttribute {
                name: "recipe_scale"
                value: 1.0
            }
            PathQuad {
                x: screen.width/2.4
                y: screen.height/2.16
                controlX: screen.width*0.5
                controlY: screen.height/1.9
            }
            PathAttribute {
                name: "recipe_z"
                value: 0
            }
            PathAttribute {
                name: "recipe_scale"
                value: 0.5
            }
            PathQuad {
                x: screen.width/2.4
                y: screen.height/2
                controlX: -screen.width/11 // TODO: cuenta para las distintos numeros de peliculas.
                controlY: screen.height/1.8
            }
        }

        Timer {
            id: enabledDelay
            running: false
            repeat: false
            interval: 300
            onTriggered: { view.enabled = true }
        }

        MouseArea {
            onClicked: view.forceActiveFocus()
            anchors.fill: parent
        }

        states: [
            State {
                name: "show"
            },
            State {
                name: "hide"
            },
            State {
                name: "hide_viewer_show_finder"
            }
        ]
        transitions: [
            Transition {
                to: "show"
                SequentialAnimation {
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: networkTimeOut; property: "running"; value: false }
                    PropertyAction { target: viewerHandler; property: "location_block"; value: 0 }
                    PropertyAction { target: viewerHandler; property: "location_global"; value: 0 }
                    NumberAnimation { duration: 250 }
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.5; duration: 1200; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 96; duration: 1200; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: view; property: "enabled"; value: true }
                }
            },
            Transition {
                to: "hide"
                SequentialAnimation {
                    PropertyAction { target: view; property: "enabled"; value: false }
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: screen.width+50; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 600; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: update_data; property: "running"; value: true }
                }
            },
            Transition {
                to: "hide_viewer_show_finder"
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: screen.width+50; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: screen; property: "state"; value: "show_finder" }
                }
            }
        ]

        Keys.onPressed: {
            if (event.key === Qt.Key_Up) {
                if(root.total_films > 5 && view.counter < root.total_films) {
                    viewerHandler.offset = viewerHandler.offset+5
                    view.counter = view.counter+5
                    current_film = view.counter-4
                    view.state = "hide"
                }
            } else if (event.key === Qt.Key_Down) {
                if(root.total_films > 5 && current_film-5 > 0) {
                    viewerHandler.offset = viewerHandler.offset-5
                    view.counter = view.counter-5
                    current_film = view.counter-4
                    view.state = "hide"
                }
            } else if (event.key === Qt.Key_Right) {
                view.enabled = false
                enabledDelay.start()
                incrementCurrentIndex()
                if (location_global === root.data_films.length-12) {
                    location_global = 0
                    location_block = 0
                    current_film = current_film-root.block_films+1
                } else {
                    location_global = location_global+12
                    ++current_film
                    ++location_block
                }
            } else if (event.key === Qt.Key_Left) {
                view.enabled = false
                enabledDelay.start()
                decrementCurrentIndex()
                if (location_global === 0) {
                    location_global = root.data_films.length-12
                    location_block = root.block_films-1
                    current_film = current_film+root.block_films-1
                } else {
                    location_global = location_global-12
                    --current_film
                    --location_block
                }
            } else if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier) && !networkTimeOut.running) {
                screen.state = "show_help"
                event.accepted = true
            } else if (event.key === Qt.Key_Escape) {
                view.state = "hide_viewer_show_finder"
                event.accepted = true;
            } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                root.signal_qml_quit()
                event.accepted = true;
            }
        }
    
        onEnabledChanged: { if (view.enabled) view.forceActiveFocus() }
        onFocusChanged: { if (!view.focus) view.forceActiveFocus() }
    }

    Component {
        id: delegate
        Item {
            id: recipe
            z: PathView.recipe_z
            width: view.width
            height: view.height
            scale: PathView.recipe_scale

            Cover {
                id: cover
                width: parent.width/4.58
                height: parent.height/1.8
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -parent.width/15.86
                anchors.verticalCenterOffset: -parent.height/270
            }
            Layer {
                id: layer
                width: screen.width/1.86
                visible: recipe.PathView.isCurrentItem
                enabled: recipe.PathView.isCurrentItem
                anchors.top: parent.top
                anchors.left: cover.right
                anchors.bottom: parent.bottom
            }
        }
    }

    Timer {
        id: networkTimeOut
        running: false
        repeat: false
        interval: 60000
        onTriggered: { root.errorNetwork = true }
    }
    Timer {
        id: update_data
        running: false
        repeat: false
        onTriggered: {
            model.clear()
            root.signal_qml_find(root.inputUser, root.pornstar, root.category, root.quality, root.full, viewerHandler.offset, false)
        }
    }

    function append_data() {
        coverStatus = []
        var torrent
        var row = 0
        for (var i=0; i<root.block_films; i++) {
           torrent = root.data_films[row+11].split(",")
           model.append({ "title": root.data_films[row], "cast": root.data_films[row+1], "categories": root.data_films[row+2], "quality": root.data_films[row+3],
                          "full": root.data_films[row+4], "hostPreview": root.data_films[row+5], "urlPreview": root.data_films[row+6], "filePreview": root.data_films[row+7],
                          "idPreview": i, "hostCover": root.data_films[row+8], "urlFrontCover": root.data_films[row+9],"urlBackCover": root.data_films[row+10],
                          "urlTorrent": torrent[0], "scenes": Number(torrent[1]) })
           row += 12
        }
        root_loader_A.source = "../global/Network.qml"
        networkTimeOut.start()
    }

    onVisibleChanged: { if (viewerHandler.visible) view.forceActiveFocus() }
    Component.onCompleted: append_data()
}
