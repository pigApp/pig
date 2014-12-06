import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: viewer
    x: root.xB

    property bool showTorrentInformation
    property int offset: 0
    property int location: 0
    property int locationOnList
    property int currentFilm: 1
    property int nIndex: 0
    property var coverStatus: []

    ListModel { id: model }

    PathView {
        id: view
        model: model
        delegate: delegate
        interactive: false
        //cacheItemCount:0
        enabled: false
        anchors.fill: parent

        property int counter: 5

        path: Path {
            startX: screen.width/1.86
            startY: screen.height/2
            PathAttribute { name: "recipeZ"; value: 2 }
            PathAttribute { name: "recipeScale"; value: 1.0 }
            PathQuad { x: screen.width/1.86; y: 500; controlX: screen.width*1.33; controlY: screen.height/2.05 } // TODO: Pasar 500 a porcentaje.
            PathAttribute { name: "recipeZ"; value: 0 }
            PathAttribute { name: "recipeScale"; value: 0.5 }
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
                name: "show_torrentInformation"
                when: showTorrentInformation
            },
            State {
                name: "hide_torrentInformation"
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
                    PropertyAction { target: viewer; property: "location"; value: 0 }
                    PropertyAction { target: viewer; property: "locationOnList"; value: 0 }
                    PropertyAction { target: root; property: "stopPreview"; value: false }
                    NumberAnimation { duration: 250 }
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 600; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 64; duration: 2000; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: enabledDelay; property: "running"; value: true }
                }
            },
            Transition {
                to: "hide"
                SequentialAnimation {
                    PropertyAction { target: view; property: "enabled"; value: false }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: screen.width+50; duration: 600; easing.type: Easing.OutQuart }
                    PropertyAction { target: root; property: "stopPreview"; value: true }
                    PropertyAction { target: update_data; property: "running"; value: true }
                }
            },
            Transition {
                to: "show_torrentInformation"
                PropertyAction { target: root; property: "screenWidth"; value: screen.width }
                PropertyAction { target: viewer; property: "showTorrentInformation"; value: false }
                PropertyAction { target: view; property: "enabled"; value: false }
                PropertyAction { target: root_loader_A; property: "source"; value: "TorrentInformation.qml" }
                SequentialAnimation {
                    //NumberAnimation { duration: 20 }
                    NumberAnimation { target: viewer; easing.amplitude: 1.7; properties: "x"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root; property: "stopPreview"; value: true }
                }
            },
            Transition {
                to: "hide_torrentInformation"
                SequentialAnimation {
                    PropertyAction { target: viewer; property: "x"; value: -root.screenWidth }
                    NumberAnimation { target: viewer; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root; property: "stopPreview"; value: false }
                }
            },
            Transition {
                to: "hide_viewer_show_finder"
                SequentialAnimation {
                    PropertyAction { target: root; property: "stopPreview"; value: true }
                    //NumberAnimation { duration: 20 }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: screen.width+50; duration: 600; easing.type: Easing.OutQuart }
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
                root.stopPreview = true
                root.signal_qml_quit()
                event.accepted = true;
            }
        }

        Keys.onUpPressed: {
            if(totalFilms > 5 && counter < totalFilms) {
                viewer.offset = viewer.offset+5
                counter = counter+5
                currentFilm = counter-4
                view.state = "hide"
            }
        }
        Keys.onDownPressed: {
            if(totalFilms > 5 && currentFilm-5 > 0) {
                viewer.offset = viewer.offset-5
                counter = counter-5
                currentFilm = counter-4
                view.state = "hide"
            }
        }
        Keys.onRightPressed: {
            view.enabled = false
            enabledDelay.start()
            incrementCurrentIndex()
            if (locationOnList === dataFilms.length-11) {
                locationOnList = 0
                location = 0
                currentFilm = currentFilm-nFilms+1
            } else {
                locationOnList = locationOnList+11
                ++currentFilm
                ++location
            }

        }
        Keys.onLeftPressed: {
            view.enabled = false
            enabledDelay.start()
            decrementCurrentIndex()
            if (locationOnList === 0) {
                locationOnList = dataFilms.length-11
                location = nFilms-1
                currentFilm = currentFilm+nFilms-1
            } else {
                locationOnList = locationOnList-11
                --currentFilm
                --location
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
            z: PathView.recipeZ
            scale: PathView.recipeScale
            width: view.width
            height: view.height

            /*
            Item {
                id: previewBox
                width: screen.width/3.04
                height: screen.height/2.51
                visible: recipe.PathView.isCurrentItem//
                //visible: false//
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: parent.height/710
                Component.onCompleted: {
                    Preview {
                        host: hostPreview
                        url: urlPreview
                        target: filePreview
                        id: idPreview
                    }

                    var component = Qt.createComponent("Preview.qml")
                    var object = component.createObject(previewBox)
                    object.host = hostPreview
                    object.url = urlPreview
                    object.target = filePreview
                    object.id = idPreview
                }
            }
             */

            /*
            RectangularGlow {
                id: coverEffect
                color: "black"
                glowRadius: 35
                spread: 0.2
                cornerRadius: 40
                anchors.fill: frontImage//cover
            }
            */



            /*
            Rectangle { // TODO: Mover afuera del recipiente.
                id: dateLayer
                color: Qt.rgba(1, 1, 1, 0.02)
                visible: recipe.PathView.isCurrentItem
                anchors.right: flipableCover.right
                anchors.left: flipableCover.left
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.bottom: flipableCover.top
                anchors.bottomMargin: 10
            }
            */

            Rectangle {
                id: layer
                color: Qt.rgba(1, 1, 1, 0.02)
                anchors.top: parent.top
                anchors.right: flipableCover.right
                anchors.left: flipableCover.left
                anchors.bottom: parent.bottom
            }

            Column {
                id: datesColumn
                spacing: parent.height/216
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.right: flipableCover.right
                anchors.left: flipableCover.left
                anchors.leftMargin: 30
                anchors.bottom: flipableCover.top
                Text {
                    id: castLabel
                    text: cast
                    color: "white"
                    font.family: finderFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: categoriesLabel
                    text: categories
                    color: "white"
                    font.family: finderFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: qualityLabel
                    text: quality
                    color: "white"
                    font.family: finderFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: splitLabel
                    text: "SPLIT"
                    color: { if (scenes !== 1) "white"; else Qt.rgba(1, 1, 1, 0.1) }
                    font.family: finderFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
                Text {
                    id: fullLabel
                    text: "FULL"
                    color: { if (full === "") "white"; else Qt.rgba(1, 1, 1, 0.1) }
                    font.family: finderFont.name
                    font.letterSpacing: screen.width/480
                    font.wordSpacing: -screen.width/384
                    font.pixelSize: screen.height/38
                }
            }

            Flipable {
                id: flipableCover
                width: front.width
                height: front.height
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -parent.width/15.86
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -parent.height/270
                property bool flipped
                front: Image {
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
                                   frontCover.source = "/resources/images/viewer/NOT_AVAILABLE/cover_NOT_AVAILABLE.jpg"
                                   coverStatus.push(1)
                                   check_cover_status();
                               }
                           }
                       }
                back: Image {
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
                transform: Rotation {
                    id: rotation
                    origin.x: flipableCover.width / 2
                    origin.y: flipableCover.height / 2
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
                    onClicked: { flipableCover.flipped = !flipableCover.flipped }
                }
            }

            Loader {
                id: previewLoader
                source: "Preview.qml"
                asynchronous: true
                visible: { status === Loader.Ready && recipe.PathView.isCurrentItem }
                anchors.top: flipableCover.bottom
                anchors.right: flipableCover.right
                anchors.left: flipableCover.left
                anchors.bottom: parent.bottom
                onStatusChanged: {
                    if (status === Loader.Ready) {
                        previewLoader.item.host = hostPreview
                        previewLoader.item.url = urlPreview
                        previewLoader.item.target = filePreview
                        previewLoader.item.id = idPreview
                    }
                }
            }

           /*
            Row {
                id: openSceneRow
                spacing: 1
                visible: false
                //visible: recipe.PathView.isCurrentItem//
                anchors.left: cover.right
                anchors.leftMargin: parent.width/42.66
                anchors.top: datesColumn.bottom
                anchors.topMargin: parent.height/38.57
                Component.onCompleted: {
                    for (var i=1; i<=scenes && i<=8; i++) {
                        var component = Qt.createComponent("ButtonScene.qml")
                        var object = component.createObject(openSceneRow)
                        object.magnet = magnet
                        object.scene = i
                    }
                }
            }
            */

            Row {
                id: counterRow
                spacing: parent.width/480
                visible: false//
                anchors.right: parent.right
                anchors.rightMargin: parent.width/480
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -parent.height/135
                Text {
                    id: currentLabel
                    text: currentFilm
                    color: "white"
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    id: totalLabel
                    text: totalFilms
                    color: Qt.rgba(1, 1, 1, 0.6)
                    font.family: pigFont.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    id: arrowsLabel
                    text: {
                        if (currentFilm == 5 && totalFilms > currentFilm)
                            " ⇈"
                        else if (currentFilm !=5 && currentFilm == totalFilms)
                            " ⇊"
                        else if (currentFilm%5 == 0 && currentFilm !=5 && currentFilm != totalFilms)
                            " ⇅"
                        else
                            " ⇄"
                    }
                    color: Qt.rgba(1, 1, 1, 0.6)
                    font.family: pigFont.name
                    font.pixelSize: screen.height/54
                    anchors.verticalCenter: totalLabel.verticalCenter
                    anchors.verticalCenterOffset: screen.height/360
                }
            }
        }
    }

    Timer {
        id: timeOutNetwork
        running: false
        repeat: false
        interval: 60000
        onTriggered: { root.errorNetwork = true }
    }

    function append_data() {
        coverStatus = []
        var torrent
        var row = 0
        for (var i=0; i<nFilms; i++) {
           torrent = dataFilms[row+10].split(",")
           model.append({ "cast": dataFilms[row], "categories": dataFilms[row+1], "quality": dataFilms[row+2], "full": dataFilms[row+3], "hostPreview": dataFilms[row+4],
                          "urlPreview": dataFilms[row+5], "filePreview": dataFilms[row+6], "idPreview": i, "hostCover": dataFilms[row+7], "urlFrontCover": dataFilms[row+8],
                          "urlBackCover": dataFilms[row+9], "magnet": torrent[0], "scenes": Number(torrent[1]) })
           row += 11
        }
        root_loader_A.source = "Network.qml"
        timeOutNetwork.start()
    }

    Timer {
        id: update_data
        running: false
        repeat: false
        onTriggered: {
            model.clear()
            root.signal_qml_find(root.input, root.pornstar, root.category, root.quality, root.full, viewer.offset, false)
        }
    }

    function check_cover_status() {
        if (coverStatus.length === nFilms)
            view.state = "show"
    }

    Connections {
        target: cppSignals
        onSignal_hide_torrent_information: { view.state = "hide_torrentInformation" }
    }

    Component.onCompleted: append_data()
}
