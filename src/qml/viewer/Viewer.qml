import QtQuick 2.4

import "preview/"

Item {
    id: viewer
    x: root.xb

    property bool updateData
    property int n_blockMovies: 5
    property int locationOnBlockMovies: 0
    property int currentMovie: 1
    property var coverStatus: []
    property var previewStatus: []

    ListModel { id: model }

    Rectangle {
        id: layerPreview
        width: parent.width/3
        height: parent.height/2.25
        color: Qt.rgba(1, 1, 1, 0.02)
        anchors.right: parent.right
        anchors.rightMargin: parent.width/16.69
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/270
    }

    PathView {
        id: view
        model: model
        delegate: delegate
        cacheItemCount: 0
        interactive: false
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
                controlX: screen.width/1.6
                controlY: screen.height/1.8
            }
        }

        Timer {
            id: delayEnable
            running: false
            repeat: false
            interval: 300
            onTriggered: { view.enabled = true }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: view.forceActiveFocus()
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
                    PropertyAction { target: viewer; property: "locationOnBlockMovies"; value: 0 }
                    NumberAnimation { duration: 250 }
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: 0; duration: 1200; easing.type: Easing.OutQuart }
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.8; duration: 1200; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 64; duration: 1200; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: view; property: "enabled"; value: true }
                }
            },
            Transition {
                to: "hide"
                SequentialAnimation {
                    PropertyAction { target: view; property: "enabled"; value: false }
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: screen.width+10; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenA"; to: 0.4; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 32; duration: 600; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: viewer; property: "updateData"; value: true }
                }
            },
            Transition {
                to: "hide_viewer_show_finder"
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xb"; to: screen.width+10; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: screen; property: "state"; value: "show_finder" }
                }
            }
        ]

        Keys.onPressed: {
            if (event.key === Qt.Key_Up) {
                if ((root.n_movies > 5) && (view.counter < root.n_movies)) {
                    if ((root.n_movies-view.counter) >= 5 ) {
                        view.counter = view.counter+5
                    } else {
                        n_blockMovies = root.n_movies-view.counter
                        view.counter = view.counter+n_blockMovies
                    }
                    currentMovie = (view.counter-n_blockMovies)+1
                    view.state = "hide"
                }
            } else if (event.key === Qt.Key_Down) {
                if ((root.n_movies > 5) && ((view.counter-5) > 0)) {
                    view.counter = view.counter-n_blockMovies
                    currentMovie = view.counter-4
                    n_blockMovies = 5
                    view.state = "hide"
                }
            } else if (event.key === Qt.Key_Right) {
                view.enabled = false
                delayEnable.start()
                incrementCurrentIndex()
                if (locationOnBlockMovies === (n_blockMovies-1)) {
                    locationOnBlockMovies = 0
                    currentMovie = (view.counter-n_blockMovies)+1
                } else {
                    ++locationOnBlockMovies
                    ++currentMovie
                }
            } else if (event.key === Qt.Key_Left) { // TODO: Hacia la izquierda funciona mal.
                view.enabled = false
                delayEnable.start()
                decrementCurrentIndex()
                if (locationOnBlockMovies === 0) {
                    locationOnBlockMovies = view.counter-1
                    currentMovie = view.counter
                } else {
                    --locationOnBlockMovies
                    --currentMovie
                }
            } else if ((event.key === Qt.Key_H) && (event.modifiers & Qt.ControlModifier) && !timeOutNetwork.running) {
                screen.state = "show_help"
                event.accepted = true
            } else if (event.key === Qt.Key_Escape) {
                view.state = "hide_viewer_show_finder"
                event.accepted = true
            } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
                if (previewStatus.length !== 0)
                    root.preview_quit = true
                else
                    cpp.quit()
                event.accepted = true
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
            onScaleChanged:  {
                if (scale === 1.0) {
                    preview.enabled = true
                    preview.visible = true
                } else {
                    preview.enabled = false
                    preview.visible = false
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
                color: "#FD2790"
                font.family: fontGlobal.name
                font.bold: true
                font.pixelSize: screen.height/46
                visible: recipe.PathView.isCurrentItem
                anchors.left: cover.left
                anchors.bottom: cover.top
                anchors.bottomMargin: parent.height/108
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
                enabled: recipe.PathView.isCurrentItem
                visible: recipe.PathView.isCurrentItem
                anchors.left: cover.right
                anchors.leftMargin: parent.width/128
                anchors.bottom: cover.bottom
                anchors.bottomMargin: -40//-35 //-parent.height/830.76
            }
            Preview {
                id: preview
                width: parent.width/3
                height: parent.height/2.25
                anchors.right: parent.right
                anchors.rightMargin: parent.width/16.69
                anchors.verticalCenter: cover.verticalCenter
            }
        }
    }

    Timer {
        id: timeOutNetwork
        running: false
        repeat: false
        interval: 60000
        onTriggered: { root.network_err = true }
    }

    function append_data() {
        coverStatus = []
        previewStatus = []
        var torrent
        var row = (currentMovie-1)*13
        if (root.n_movies < 5) {
            n_blockMovies = root.n_movies
            view.counter = root.n_movies
        }
        for (var i=0; i<n_blockMovies; i++) {
            torrent = root.data_movies[row+12].split(",")
            model.append({ "id_movie": root.data_movies[row], "title": root.data_movies[row+1], "cast": root.data_movies[row+2], "categories": root.data_movies[row+3],
                           "quality": root.data_movies[row+4], "time": root.data_movies[row+5], "full": root.data_movies[row+6], "hostPreview": root.data_movies[row+7],
                           "urlPreview": root.data_movies[row+8], "id_preview": i, "hostCover": root.data_movies[row+9], "urlCoverFront": root.data_movies[row+10],
                           "urlCoverBack": root.data_movies[row+11], "urlTorrent": torrent[0], "scenes": Number(torrent[1]) })
            row += 13
        }
        loader_root.source = "../global/Network.qml"
        timeOutNetwork.start()
    }

    onVisibleChanged: { if (visible) view.forceActiveFocus() }
    onUpdateDataChanged: {
        if (updateData) {
            updateData = false
            model.clear()
            append_data()
        }
    }

    Component.onCompleted: append_data()
}
