import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: viewer
    x: root.xB

    property bool successCover
    property bool showTorrentInformation
    property int location: 0
    property int locationOnList
    property int currentFilm: 1
    property int nIndex: 0

    ListModel {
        id: model
    }
    ListView {
        id: listView
        width: screen.width
        height: screen.height
        visible: false
        model: model
        delegate: delegate
    }
    Component {
        id: delegate
        Item {
            id: recipe
            z: { PathView.recipeZ || 1 }
            width: listView.width
            height: listView.height
            //visible: recipe.PathView.isCurrentItem
            opacity: { PathView.recipeOpacity || 1 }
            scale: { PathView.recipeScale || 1 }

            Rectangle {
                id: translucentLayer
                width: parent.width
                height: parent.height
                color: "black"
                visible: false
                anchors.centerIn: parent
            }

            Text {
                id: titleLabel
                text: title
                color: "white"
                font.family: pigFont.name
                font.letterSpacing: -parent.width/192
                font.pixelSize: parent.height/5.75
                visible: false
                //visible: recipe.PathView.isCurrentItem//
                anchors.left: parent.left
                anchors.leftMargin: parent.width/960
                anchors.bottom: cover.top
            }

            /*
            Image {
                id: frame
                width: parent.width
                height: parent.height/2.16
                source: "/resources/images/viewer/frame.png"
                sourceSize.width: 1920
                sourceSize.height: 500
                anchors.centerIn: parent
            }

            Rectangle { // TODO: NO IRIA
                id: frame
                width: parent.width
                height: parent.height/2.16
                color: Qt.rgba(0, 0, 0, 0)
                visible: false
                anchors.centerIn: parent
            }
            */

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
                    var component = Qt.createComponent("Preview.qml")
                    var object = component.createObject(previewBox)
                    object.host = hostPreview
                    object.url = urlPreview
                    object.target = filePreview
                    object.id = idPreview
                }
            }

            RectangularGlow {
                id: coverEffect
                color: "black"
                glowRadius: 35
                spread: 0.2
                cornerRadius: 40
                anchors.fill: cover
            }
            Image {
                id: cover
                width: parent.width/4.62
                height: parent.height/1.8
                source: hostPosterCover+urlCover
                sourceSize.width: 415
                sourceSize.height: 600
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -parent.width/15.86
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -parent.height/270
                onStatusChanged: {
                    if (cover.source == dataFilms[8]+dataFilms[10] && cover.status == Image.Ready) {
                        successCover = true
                        imagesStatus.start()
                    } else if (cover.status == Image.Error) {
                        cover.source = "/resources/images/viewer/NOT_AVAILABLE/cover_NOT_AVAILABLE.png"
                        successCover = true
                        imagesStatus.start()
                    }
                }
            }

            Timer {
                id: imagesStatus
                running: false
                repeat: false
                interval: 100
                onTriggered: {
                    if (successCover) {
                        root_loader_A.source = ""
                        timeOutNetwork.stop()
                        handler.state = "show"
                        handler.enabled = true
                        handler.setFocus()
                    } else {
                        imagesStatus.restart()
                    }
                }
            }

            Rectangle {
                id: dateLayer
                color: Qt.rgba(1, 1, 1, 0.02)
                //visible: false
                visible: recipe.PathView.isCurrentItem//
                anchors.left: cover.right
                anchors.right: parent.right
                anchors.top: cover.top
                //anchors.topMargin: 50//parent.height/30
                anchors.bottom: cover.bottom
                //anchors.bottomMargin: 50//parent.height/36
            }
            Column {
                id: datesColumn
                spacing: parent.height/216
                //visible: false
                visible: recipe.PathView.isCurrentItem//
                anchors.left: cover.right
                anchors.leftMargin: parent.width/54.85
                anchors.top: cover.top
                anchors.topMargin: parent.height/12
                Text {
                    id: castLabel
                    //text: "<font color='#ff0000'>∙</font> <font color='#ffffff'>"+cast+"</font>"
                    text: "<font color='transparent'>∙</font> <font color='#ffffff'>"+cast+"</font>"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: categoriesLabel
                    //text: "<font color='#ff0000'>∙</font> <font color='#ffffff'>"+categories+"</font>"
                    text: "<font color='transparent'>∙</font> <font color='#ffffff'>"+categories+"</font>"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: qualityLabel
                    //text: "<font color='#ff0000'>∙</font> <font color='#ffffff'>"+quality+"</font>"
                    text: "<font color='transparent'>∙</font> <font color='#ffffff'>"+quality+"</font>"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: splitLabel
                    text: {
                        //if (scenes === 1)
                            "<font color='transparent'>∙</font> <font color='#ffffff'>SPLIT</font>"
                        //else
                            //"<font color='#ff0000'>∙</font> <font color='#ffffff'>SPLIT</font>"
                    }
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: fullLabel
                    text: { 
                        //if (full === "NOT")
                            "<font color='transparent'>∙</font> <font color='#ffffff'>FULL</font>"
                        //else
                            //"<font color='#ff0000'>∙</font> <font color='#ffffff'>FULL</font>"
                    }
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
            }

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

            //NumberAnimation { running: recipe.PathView.isCurrentItem && viewer.x === 0; target: poster; property: "opacity"; to: 0.3; duration: 1000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem && viewer.x === 0; target: translucentLayer; property: "opacity"; to: 0.6; duration: 1500; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 1; duration: 2000; easing.type: Easing.OutElastic }

            //PropertyAction { running: !recipe.PathView.isCurrentItem || handler.state === "hide_viewer_show_finder"; target: poster; property: "opacity"; value: 0 }
            PropertyAction { running: !recipe.PathView.isCurrentItem || handler.state === "hide_viewer_show_finder"; target: translucentLayer; property: "opacity"; value: 0 }
            NumberAnimation { running: !recipe.PathView.isCurrentItem || handler.state === "hide_viewer_show_finder"; target: datesColumn; property: "opacity"; to: 0;
                              duration: 1; easing.type: Easing.InOutQuad }
        }
    }

    PathView {
        id: handler
        model: model
        delegate: delegate
        interactive: false
        enabled: false
        anchors.fill: parent

        property int offset: 0
        property int counter: 5

        path: Path {
            startX: screen.width/2
            startY: screen.height/2
            //startX: 120
            //startY: 100

            PathAttribute { name: "recipeZ"; value: 2 }
            PathAttribute { name: "recipeOpacity"; value: 1 }
            PathAttribute { name: "recipeScale"; value: 1.0 }

            //PathQuad { x: 120; y: 25; controlX: 260; controlY: 75 }
            PathQuad { x: screen.width/2; y: 360; controlX: screen.width*1.3; controlY: screen.height/2 } //1.3

            PathAttribute { name: "recipeZ"; value: 0 }
            PathAttribute { name: "recipeOpacity"; value: 0 }
            PathAttribute { name: "recipeScale"; value: 0.3 }

            //PathQuad { x: 120; y: 100; controlX: -20; controlY: 75 }
            PathQuad { x: screen.width/2; y: screen.height/2; controlX: -screen.width/3.4; controlY: screen.height/2.05 } //3.4
        }

        //PathQuad { x: screen.width/2; y: screen.height/2; controlX: screen.width+(screen.width*2.77); controlY: screen.height/2 }
        //PathQuad { x: screen.width/2; y: screen.height/2; controlX: -screen.width*2.77; controlY: screen.height/2 }

        MouseArea {
            onClicked: handler.setFocus()
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
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 500; easing.type: Easing.OutQuart }
            },
            Transition {
                to: "hide"
                SequentialAnimation {
                    NumberAnimation { duration: 20 }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: screen.width+50; duration: 500; easing.type: Easing.OutQuart }
                }
            },
            Transition {
                to: "show_torrentInformation"
                PropertyAction { target: root; property: "screenWidth"; value: screen.width }
                PropertyAction { target: viewer; property: "showTorrentInformation"; value: false }
                PropertyAction { target: handler; property: "enabled"; value: false }
                PropertyAction { target: root_loader_A; property: "source"; value: "TorrentInformation.qml" }
                SequentialAnimation {
                    NumberAnimation { duration: 20 }
                    NumberAnimation { target: viewer; easing.amplitude: 1.7; properties: "x"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root; property: "stopPreview"; value: true }
                }
            },
            Transition {
                to: "hide_torrentInformation"
                SequentialAnimation {
                    PropertyAction { target: viewer; property: "x"; value: -root.screenWidth }
                    NumberAnimation { target: viewer; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1080; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root; property: "stopPreview"; value: false }
                }
            },
            Transition {
                to: "hide_viewer_show_finder"
                SequentialAnimation {
                    PropertyAction { target: root; property: "stopPreview"; value: true }
                    NumberAnimation { duration: 20 }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: screen.width+50; duration: 500; easing.type: Easing.OutQuart }
                    PropertyAction { target: screen; property: "state"; value: "show_finder" }
                }
            }
        ]

        function setFocus() { handler.forceActiveFocus() }

        Keys.onPressed: {
            if (event.key === Qt.Key_Escape) {
                handler.state = "hide_viewer_show_finder"
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
                handler.enabled = false
                offset = offset+5
                counter = counter+5
                currentFilm = counter-4
                updateData(offset)
            }
        }
        Keys.onDownPressed: {
            if(totalFilms > 5 && currentFilm-5 > 0) {
                handler.enabled = false
                offset = offset-5
                counter = counter-5
                currentFilm = counter-4
                updateData(offset)
            }
        }
        Keys.onRightPressed: {
            handler.enabled = false
            enabledDelay.start()
            incrementCurrentIndex()
            if (locationOnList === dataFilms.length-12) {
                locationOnList = 0
                location = 0
                currentFilm = currentFilm-nFilms+1
            } else {
                locationOnList = locationOnList+12
                ++currentFilm
                ++location
            }

        }
        Keys.onLeftPressed: {
            handler.enabled = false
            enabledDelay.start()
            decrementCurrentIndex()
            if (locationOnList === 0) {
                locationOnList = dataFilms.length-12
                location = nFilms-1
                currentFilm = currentFilm+nFilms-1
            } else {
                locationOnList = locationOnList-12
                --currentFilm
                --location
            }
        }
        Timer {
            id: enabledDelay
            running: false
            repeat: false
            interval: 500
            onTriggered: { handler.enabled = true }
        }
    }

    Timer {
        id: timeOutNetwork
        running: false
        repeat: false
        interval: 60000
        onTriggered: { root.errorNetwork = true }
    }

    function appendData() {
        location = 0
        locationOnList = 0
        successCover = false
        var torrent
        var row = 0
        for(var i=0; i<nFilms; i++) {
           torrent = dataFilms[row+11].split(",")
           model.append({ "title": dataFilms[row], "cast": dataFilms[row+1], "categories": dataFilms[row+2], "quality": dataFilms[row+3], "full": dataFilms[row+4],
                          "hostPreview": dataFilms[row+5], "urlPreview": dataFilms[row+6], "filePreview": dataFilms[row+7], "idPreview": i, "hostPosterCover": dataFilms[row+8],
                          "urlPoster": dataFilms[row+9], "urlCover": dataFilms[row+10], "magnet": torrent[0], "scenes": Number(torrent[1]) })
           row += 12
        }
        root.stopPreview = false
        root_loader_A.source = "Network.qml"
        timeOutNetwork.start()
    }

    function updateData(offset) {
        root.stopPreview = true
        handler.state = "hide"
        model.clear()
        root.signal_qml_find(root.input, root.pornstar, root.category, root.quality, root.full, offset, false)
    }

    Connections {
        target: cppSignals
        onSignal_hide_torrent_information: {
            handler.state = "hide_torrentInformation"
            handler.enabled = true
            handler.setFocus()
        }
    }

    Component.onCompleted: appendData()
}
