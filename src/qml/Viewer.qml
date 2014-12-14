import QtQuick 2.3

Item {
    id: viewer
    x: root.xB

    property bool showTorrentInformation
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
            startX: screen.width/1.86
            startY: screen.height/2
            PathAttribute { name: "recipe_z"; value: 2 }
            PathAttribute { name: "recipe_scale"; value: 1.0 }
            PathAttribute { name: "recipe_opacity"; value: 1.0 }
            PathQuad { x: screen.width/1.86; y: screen.height/2.16; controlX: screen.width*1.33; controlY: screen.height/2.05 }
            PathAttribute { name: "recipe_z"; value: 0 }
            PathAttribute { name: "recipe_scale"; value: 0.5 }
            PathAttribute { name: "recipe_opacity"; value:  0.4 } // TODO: Sacarlo sino se usa la opacidad.
            PathQuad { x: screen.width/1.86; y: screen.height/2; controlX: -screen.width/3.5; controlY: screen.height/1.8 }
        }

        states: [
            State {
                name: "show"
            },
            State {
                name: "hide"
            },
            State {
                name: "show_torrent_handler"
            },
            State {
                name: "hide_torrent_handler"
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
                    PropertyAction { target: timeOutNetwork; property: "running"; value: false }
                    PropertyAction { target: viewer; property: "location_block"; value: 0 }
                    PropertyAction { target: viewer; property: "location_global"; value: 0 }
                    NumberAnimation { duration: 250 }
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "screenOpacity"; to: 0.5; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 96; duration: 600; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: enabledDelay; property: "running"; value: true }
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
                to: "show_torrent_handler"
                //PropertyAction { target: root; property: "screenWidth"; value: screen.width }
                //PropertyAction { target: viewer; property: "showTorrentInformation"; value: false }
                PropertyAction { target: view; property: "enabled"; value: false }
                PropertyAction { target: root_loader_A; property: "source"; value: "torrent/Handler.qml" } //
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
            },
            Transition {
                to: "hide_torrent_handler"
                SequentialAnimation {
                    //PropertyAction { target: viewer; property: "x"; value: -root.screenWidth }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
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
            if (event.key === Qt.Key_Escape) {
                view.state = "hide_viewer_show_finder"
                event.accepted = true;
            } else if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier) && !timeOutNetwork.running) {
                screen.state = "show_help"
                event.accepted = true
            } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                root.signal_qml_quit()
                event.accepted = true;
            }
        }

        Keys.onUpPressed: {
            if(total_films > 5 && counter < total_films) {
                viewer.offset = viewer.offset+5
                counter = counter+5
                current_film = counter-4
                view.state = "hide"
            }
        }
        Keys.onDownPressed: {
            if(total_films > 5 && current_film-5 > 0) {
                viewer.offset = viewer.offset-5
                counter = counter-5
                current_film = counter-4
                view.state = "hide"
            }
        }
        Keys.onRightPressed: {
            view.enabled = false
            enabledDelay.start()
            incrementCurrentIndex()
            if (location_global === data_films.length-11) {
                location_global = 0
                location_block = 0
                current_film = current_film-block_films+1
            } else {
                location_global = location_global+11
                ++current_film
                ++location_block
            }

        }
        Keys.onLeftPressed: {
            view.enabled = false
            enabledDelay.start()
            decrementCurrentIndex()
            if (location_global === 0) {
                location_global = data_films.length-11
                location_block = block_films-1
                current_film = current_film+block_films-1
            } else {
                location_global = location_global-11
                --current_film
                --location_block
            }
        }

        Timer {
            id: enabledDelay
            running: false
            repeat: false
            interval: 300
            onTriggered: {
                view.enabled = true
                view.forceActiveFocus()
            }
        }

        MouseArea {
            onClicked: view.forceActiveFocus()
            anchors.fill: parent
        }
    }

    Component {
        id: delegate
        Item {
            id: recipe
            z: PathView.recipe_z
            width: view.width
            height: view.height
            scale: PathView.recipe_scale
            opacity: PathView.recipe_opacity

            Rectangle {
                id: datesLayer
                color: Qt.rgba(1, 1, 1, 0.02)
                anchors.top: parent.top
                anchors.right: flipableCover.right
                anchors.left: flipableCover.left
                anchors.bottom: flipableCover.top
            }
            Column {
                id: datesColumn
                spacing: parent.height/216
                anchors.left: datesLayer.left
                anchors.leftMargin: parent.width/64
                anchors.bottom: scenesRow.top
                anchors.bottomMargin: parent.height/108
                Text {
                    id: castLabel
                    text: cast
                    color: "white"
                    font.family: customFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: categoriesLabel
                    text: categories
                    color: "white"
                    font.family: customFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: qualityLabel
                    text: quality
                    color: "white"
                    font.family: customFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: splitLabel
                    text: "SPLIT"
                    color: { if (scenes !== 1) "white"; else Qt.rgba(1, 1, 1, 0.1) }
                    font.family: customFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: fullLabel
                    text: "FULL"
                    color: { if (full === "") "white"; else Qt.rgba(1, 1, 1, 0.1) }
                    font.family: customFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
            }

            Row {
                id: scenesRow
                spacing: parent.width/192
                visible: recipe.PathView.isCurrentItem
                enabled: recipe.PathView.isCurrentItem
                anchors.right: datesLayer.right
                anchors.rightMargin: parent.width/96
                anchors.bottom: datesLayer.bottom
                anchors.bottomMargin: parent.height/108
                Repeater {
                    model: scenes
                    delegate:
                    Image {
                        id: openSceneIcon
                        width: screen.width/60
                        height: screen.height/42.63
                        opacity: 0.1
                        sourceSize.width: screen.width/60
                        sourceSize.height: screen.height/42.63
                        source: "qrc:/img-open"
                        MouseArea {
                            hoverEnabled: true
                            onEntered: { openSceneIcon.state = "in" }
                            onHoveredChanged: { openSceneIcon.state = "out" }
                            onClicked: {
                                view.state = "show_torrent_handler"
                                root.signal_qml_torrent_handler(magnet, index, false)
                            }
                            anchors.fill: parent
                        }
                        states: [
                            State {
                                name: "in"
                            },
                            State {
                                name: "out"
                            }
                        ]
                        transitions: [
                            Transition {
                                to: "in"
                                NumberAnimation { target: openSceneIcon; easing.amplitude: 1.7; properties: "opacity"; to: 1; duration: 100; easing.type: Easing.OutQuart }
                            },
                            Transition {
                                to: "out"
                                NumberAnimation { target: openSceneIcon; easing.amplitude: 1.7; properties: "opacity"; to: 0.1; duration: 100; easing.type: Easing.OutQuart }
                            }
                        ]
                    }
                }
            }

            Flipable {
                id: flipableCover
                width: front.width
                height: front.height
                front:
                Image {
                    id: frontCover
                    width: screen.width/4.58
                    height: screen.height/1.8
                    sourceSize.width: 419
                    sourceSize.height: 600
                    source: hostCover+urlFrontCover
                    onStatusChanged: {
                        if (frontCover.status == Image.Ready) {
                            coverStatus.push(0)
                            check_cover_status();
                        } else if (frontCover.status == Image.Error) {
                            frontCover.source = "qrc:/img-cover_na"
                            coverStatus.push(1)
                            check_cover_status();
                        }
                    }
                }
                back:
                Image {
                    id: backCover
                    width: frontCover.width
                    height: frontCover.height
                    sourceSize.width: 419
                    sourceSize.height: 600
                    source: {
                        if (urlBackCover !== "")
                            hostCover+urlBackCover
                        else
                            frontCover.source
                    }
                    onStatusChanged: {
                        if (backCover.status == Image.Error)
                            backCover.source = frontCover.source
                    }
                }
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -parent.height/270
                anchors.horizontalCenterOffset: -parent.width/15.86

                property bool flipped

                transform: Rotation {
                    id: rotation
                    origin.x: flipableCover.width/2
                    origin.y: flipableCover.height/2
                    axis.x: 0
                    axis.y: 1
                    axis.z: 0
                }
                states: State {
                    name: "back"
                    when: flipableCover.flipped
                    PropertyChanges { target: rotation; angle: 180 }
                }
                transitions: Transition {
                    ParallelAnimation {
                        NumberAnimation { target: rotation; properties: "angle"; duration: 600 }
                        SequentialAnimation {
                            NumberAnimation { target: flipableCover; property: "scale"; to: 0.75; duration: 300 }
                            NumberAnimation { target: flipableCover; property: "scale"; to: 1.0; duration: 300 }
                        }
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        flipableCover.flipped = !flipableCover.flipped
                        view.forceActiveFocus()
                    }
                }
            }

            Rectangle {
                id: previewLayer
                color: Qt.rgba(1, 1, 1, 0.05)
                visible: recipe.PathView.isCurrentItem
                anchors.top: flipableCover.bottom
                anchors.right: flipableCover.right
                anchors.left: flipableCover.left
                anchors.bottom: parent.bottom
                Image {
                    id: previewIcon
                    width: screen.width/60
                    height: screen.height/33.75
                    sourceSize.width: screen.width/60
                    sourceSize.height: screen.height/33.75
                    source: "qrc:/img-play"
                    opacity: 0.1
                    anchors.centerIn: parent
                }
                Text {
                    id: previewLabel
                    text: "PREVIEW"
                    color: Qt.rgba(1, 1, 1, 0.06)
                    font.family: globalFont.name
                    font.bold: true
                    font.pixelSize: screen.height/74
                    anchors.top: previewIcon.bottom
                    anchors.horizontalCenter: previewLayer.horizontalCenter
                }
                MouseArea {
                    onClicked: {
                        if (previewIcon.visible) {
                            previewIcon.visible = false
                            previewLabel.text = ""
                            preview_loader.source = "Preview.qml"
                            preview_loader.active = true
                            view.forceActiveFocus()
                        }
                    }
                    anchors.fill: parent
                }
            }
            Loader {
                id: preview_loader
                active: false
                asynchronous: true
                visible: { status === Loader.Ready && recipe.PathView.isCurrentItem }
                anchors.fill: previewLayer
                onStatusChanged: {
                    if (status === Loader.Ready) {
                        preview_loader.item.host = hostPreview
                        preview_loader.item.url = urlPreview
                        preview_loader.item.target = filePreview
                        preview_loader.item.id = idPreview
                    }
                }
            }
        }
    }
    Row {
        id: counterRow
        spacing: parent.width/480
        anchors.right: parent.right
        anchors.rightMargin: parent.width/128
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/270
        Text {
            id: currentFilmLabel
            text: current_film
            color: { if (location_block+1 !== block_films) Qt.rgba(1, 1, 1, 0.1); else "white" }
            font.family: globalFont.name
            font.pixelSize: screen.height/38
        }
        Text {
            id: totalFilmsLabel
            text: total_films
            color: { if (current_film !== total_films) Qt.rgba(1, 1, 1, 0.1); else "white" }
            font.family: globalFont.name
            font.pixelSize: screen.height/38
        }
    }

    Timer {
        id: timeOutNetwork
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
            root.signal_qml_find(root.inputUser, root.pornstar, root.category, root.quality, root.full, viewer.offset, false)
        }
    }

    function append_data() {
        coverStatus = []
        var torrent
        var row = 0
        for (var i=0; i<block_films; i++) {
           torrent = data_films[row+10].split(",")
           model.append({ "cast": data_films[row], "categories": data_films[row+1], "quality": data_films[row+2], "full": data_films[row+3], "hostPreview": data_films[row+4],
                          "urlPreview": data_films[row+5], "filePreview": data_films[row+6], "idPreview": i, "hostCover": data_films[row+7], "urlFrontCover": data_films[row+8],
                          "urlBackCover": data_films[row+9], "magnet": torrent[0], "scenes": Number(torrent[1]) })
           row += 11
        }
        root_loader_A.source = "global/Network.qml"
        timeOutNetwork.start()
    }
    function check_cover_status() {
        if (coverStatus.length === block_films)
            view.state = "show"
    }

    Component.onCompleted: append_data()
}
