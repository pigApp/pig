import QtQuick 2.4
import "preview/"

Item {
    id: viewerHandler
    x: root.xb

    property int n_block_films: 5
    property int block_films_location: 0
    property int current_film: 1
    property var coverStatus: []
    property var previewStatus: []

    ListModel { id: model }

    PathView {
        id: view
        model: model
        delegate: delegate
        cacheItemCount: 0
        maximumFlickVelocity: 600
        enabled: false
        anchors.fill: parent

        property int counter: 5

        path: Path {
            startX: screen.width/2
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
                x: screen.width/2
                y: screen.height/2.16
                controlX: -screen.width/19.2
                controlY: screen.height/1.9
            }
            PathAttribute {
                name: "recipe_z"
                value: 0
            }
            PathAttribute {
                name: "recipe_scale"
                value: 0.3
            }
            PathQuad {
                x: screen.width/2
                y: screen.height/2
                controlX: screen.width/1.55
                controlY: screen.height/1.8
            }
        }

        Timer {
            id: delayEnabled
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
                    PropertyAction { target: loader_root; property: "source"; value: "" }
                    PropertyAction { target: timeOutNetwork; property: "running"; value: false }
                    PropertyAction { target: viewerHandler; property: "block_films_location"; value: 0 }
                    NumberAnimation { duration: 250 }
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
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
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: screen.width+50; duration: 600; easing.type: Easing.OutQuart }
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
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: screen.width+50; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: screen; property: "state"; value: "show_finder" }
                }
            }
        ]

        Keys.onPressed: {
            if (event.key === Qt.Key_Up) {
                if ((root.n_films > 5) && (view.counter < root.n_films)) {
                    if ((root.n_films-view.counter) >= 5 ) {
                        view.counter = view.counter+5
                    } else {
                        n_block_films = root.n_films-view.counter
                        view.counter = view.counter+n_block_films
                    }
                    current_film = (view.counter-n_block_films)+1
                    view.state = "hide"
                }
            } else if (event.key === Qt.Key_Down) {
                if ((root.n_films > 5) && ((view.counter-5) > 0)) {
                    view.counter = view.counter-n_block_films
                    current_film = view.counter-4
                    n_block_films = 5
                    view.state = "hide"
                }
            } else if (event.key === Qt.Key_Right) {
                view.enabled = false
                delayEnabled.start()
                incrementCurrentIndex()
                if (block_films_location === (n_block_films-1)) {
                    block_films_location = 0
                    current_film = (view.counter-n_block_films)+1
                } else {
                    ++block_films_location
                    ++current_film
                }
            } else if (event.key === Qt.Key_Left) { // TODO: Hacia la izquierda funciona mal.
                view.enabled = false
                delayEnabled.start()
                decrementCurrentIndex()
                if (block_films_location === 0) {
                    block_films_location = view.counter-1
                    current_film = view.counter
                } else {
                    --block_films_location
                    --current_film
                }
            } else if ((event.key === Qt.Key_H) && (event.modifiers & Qt.ControlModifier) && !timeOutNetwork.running) {
                screen.state = "show_help"
                event.accepted = true
            } else if (event.key === Qt.Key_Escape) {
                view.state = "hide_viewer_show_finder"
                event.accepted = true;
            } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
                if (previewStatus.length !== 0)
                    root.abort_preview_quit = true
                else
                    cpp.quit()
                event.accepted = true;
            }
        }
    
        onEnabledChanged: { if (view.enabled) view.forceActiveFocus() }
        onFocusChanged: { if (!view.focus) view.forceActiveFocus() }
    }

    Component {
        id: delegate
        Item { // TODO: visible y enable si no se esta cargando un torrent (root.onShowTorrent)
               // TODO: detener el player y descarga (preview) si se sale (Ctrl-V).
            id: recipe
            z: PathView.recipe_z
            width: view.width
            height: view.height
            scale: PathView.recipe_scale
            onScaleChanged:  {
                if (recipe.scale === 1.0) {
                    preview.visible = true
                    preview.enabled = true
                } else {
                    preview.visible = false
                    preview.enabled = false
                }
            }
            Text {
                id: labelTitle
                text: title
                color: "white"
                font.family: fontGlobal.name
                font.pixelSize: screen.height/10
                visible: recipe.PathView.isCurrentItem
                anchors.left: cover.left
                anchors.bottom: labelCast.top
                anchors.bottomMargin: -parent.height/54
            }
            Text {
                id: labelCast
                text: cast
                color: Qt.rgba(1, 1, 1, 0.5)
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/54
                visible: recipe.PathView.isCurrentItem
                anchors.left: cover.left
                anchors.bottom: cover.top
                anchors.bottomMargin: parent.height/108
            }
            Row {
                visible: recipe.PathView.isCurrentItem
                anchors.left: cover.right
                anchors.leftMargin: 10
                anchors.top: cover.top
                anchors.topMargin: -15
                Text {
                    id: labelCurrentFilm
                    text: current_film
                    color: "white"
                    font.family: fontGlobal.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    text: "·"
                    color: "white"
                    font.family: fontGlobal.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    id: labelTotalFilms
                    text: root.n_films
                    color: "white"
                    font.family: fontGlobal.name
                    font.pixelSize: screen.height/23
                }
            }
            Cover {
                id: cover
                width: parent.width/4.58
                height: parent.height/1.8
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -parent.width/15.86
                anchors.verticalCenterOffset: -parent.height/270
            }
            Dates {
                id: dates
                visible: recipe.PathView.isCurrentItem
                anchors.top: cover.bottom
                anchors.left: cover.left
            }
            Scenes {
                id: btnScenes
                totalScenes: scenes
                visible: recipe.PathView.isCurrentItem
                enabled: recipe.PathView.isCurrentItem
                anchors.left: cover.right
                anchors.leftMargin: parent.width/112.94
                anchors.bottom: cover.bottom 
            }
            Preview {
                id: preview
                width: parent.width/3
                height: parent.height/2.25
                anchors.right: parent.right
                anchors.rightMargin: parent.width/16.69
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -parent.height/27
            }
        }
    }
    Image {
        id: board
        width: parent.width/3
        height: parent.height/2.25
        sourceSize.width: board.width
        sourceSize.height: board.height
        source: "qrc:/img-board"
        visible: false
        anchors.right: parent.right
        anchors.rightMargin: parent.width/16.69
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/27
    }

    Timer {
        id: timeOutNetwork
        running: false
        repeat: false
        interval: 60000
        onTriggered: { root.network_err = true }
    }
    Timer {
        id: update_data
        running: false
        repeat: false
        onTriggered: {
            model.clear()
            append_data()
        }
    }

    function append_data() {
        coverStatus = []
        previewStatus = []
        var torrent
        var row = (current_film-1)*13
        if (root.n_films < 5) {
            n_block_films = root.n_films
            view.counter = root.n_films
        }
        for (var i=0; i<n_block_films; i++) {
            torrent = root.data_films[row+12].split(",")
            model.append({ "id_film": root.data_films[row], "title": root.data_films[row+1], "cast": root.data_films[row+2], "categories": root.data_films[row+3],
                           "quality": root.data_films[row+4], "time": root.data_films[row+5], "full": root.data_films[row+6], "hostPreview": root.data_films[row+7],
                           "urlPreview": root.data_films[row+8], "id_preview": i, "hostCover": root.data_films[row+9], "urlFrontCover": root.data_films[row+10],
                           "urlBackCover": root.data_films[row+11], "urlTorrent": torrent[0], "scenes": Number(torrent[1]) })
            row += 13
        }
        loader_root.source = "../global/Network.qml"
        timeOutNetwork.start()
    }

    onVisibleChanged: { if (viewerHandler.visible) view.forceActiveFocus() }
    Component.onCompleted: append_data()
}
