import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: output
    x: screen.width

    property bool successPoster
    property bool successCover
    property bool showTorrentInformation

    property int location: 0
    property int currentFilm: 1
    property int locationOnList
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
            visible: recipe.PathView.isCurrentItem
            opacity: { PathView.recipeOpacity || 1 }

            Image {
                id: poster
                width: parent.width
                height: parent.height
                source: hostPosterCover+urlPoster
                sourceSize.width: 1920
                sourceSize.height: 1080
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                onStatusChanged: {
                    if (poster.source == list[8]+list[9] && poster.status == Image.Ready) {
                        successPoster = true
                        if (!successCover)
                            imagesStatus.start()
                    } else if (poster.status == Image.Error) {
                        poster.source = "qrc:resources/images/output/NOT_AVAILABLE/poster_NOT_AVAILABLE.png"
                        successPoster = true
                        if (!successCover)
                            imagesStatus.start()
                    }
                }
            }
            Rectangle {
                id: translucedLayer
                width: parent.width
                height: parent.height
                color: "black"
                anchors.centerIn: parent
            }

            Text {
                id: titleLabel
                text: title
                color: "white"
                font.family: pigFont.name
                font.letterSpacing: -parent.width/192
                font.pixelSize: parent.height/5.75
                anchors.left: parent.left
                anchors.leftMargin: parent.width/960
                anchors.bottom: cover.top
            }

            Image {
                id: frame
                width: parent.width
                height: parent.height/2.16
                source: "/resources/images/output/frame.png"
                sourceSize.width: 1920
                sourceSize.height: 500
                anchors.centerIn: parent
            }

            Item {
                id: previewBox
                width: screen.width/3.04
                height: screen.height/2.51
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: parent.height/710
                Component.onCompleted: {
                    var component = Qt.createComponent("Preview.qml")
                    var object = component.createObject(previewBox)
                    object.host = hostPreview
                    object.url = urlPreview
                    object.file = filePreview
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
                    if (cover.source == list[8]+list[10] && cover.status == Image.Ready) {
                        successCover = true
                        if (!successPoster)
                            imagesStatus.start()
                    } else if (cover.status == Image.Error) {
                        cover.source = "/resources/images/output/NOT_AVAILABLE/cover_NOT_AVAILABLE.png"
                        successCover = true
                        if (!successPoster)
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
                    if (successPoster && successCover) {
                        timeOutNetwork.stop()
                        root_loader_A.source = ""
                        handler.state = "show"
                        handler.enabled = true
                        handler.setFocus()
                        root.stopPreview = false
                    } else {
                        imagesStatus.restart()
                    }
                }
            }

            Rectangle {
                id: dateLayer
                color: Qt.rgba(0, 0, 0, 0.4)
                anchors.left: cover.right
                anchors.right: parent.right
                anchors.top: frame.top
                anchors.topMargin: parent.height/30
                anchors.bottom: frame.bottom
                anchors.bottomMargin: parent.height/36
            }
            Column {
                id: datesColumn
                spacing: parent.height/216
                anchors.left: cover.right
                anchors.leftMargin: parent.width/54.85
                anchors.top: frame.top
                anchors.topMargin: parent.height/12
                Text {
                    id: castLabel
                    text: "<font color='#ff0000'>∙<font/> <font color='#ffffff'>"+cast+"<font/>"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: categoryLabel
                    text: "<font color='#ff0000'>∙<font/> <font color='#ffffff'>"+categories+"<font/>"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: qualityLabel
                    text: "<font color='#ff0000'>∙<font/> <font color='#ffffff'>"+quality+"<font/>"
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: splitLabel
                    text: {
                        if (scenes === 1)
                            "<font color='transparent'>∙<font/> <font color='#ffffff'>SPLIT<font/>"
                        else
                            "<font color='#ff0000'>∙<font/> <font color='#ffffff'>SPLIT<font/>"
                    }
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
                Text {
                    id: fullLabel
                    text: { 
                        if (full === "NOT")
                            "<font color='transparent'>∙<font/> <font color='#ffffff'>FULL<font/>"
                        else
                            "<font color='#ff0000'>∙<font/> <font color='#ffffff'>FULL<font/>"
                    }
                    font.family: pigFont.name
                    font.bold: true
                    font.pixelSize: screen.height/35
                }
            }
            Row {
                id: openSceneRow
                spacing: 1
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

            NumberAnimation { running: recipe.PathView.isCurrentItem && output.x === 0; target: poster; property: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem && output.x === 0; target: translucedLayer; property: "opacity"; to: 0.65; duration: 1500; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 1; duration: 2000; easing.type: Easing.OutElastic }

            PropertyAction { running: !recipe.PathView.isCurrentItem || handler.state === "hideOutput_showFinder"; target: poster; property: "opacity"; value: 0 }
            PropertyAction { running: !recipe.PathView.isCurrentItem || handler.state === "hideOutput_showFinder"; target: translucedLayer; property: "opacity"; value: 0 }
            NumberAnimation { running: !recipe.PathView.isCurrentItem || handler.state === "hideOutput_showFinder"; target: datesColumn; property: "opacity"; to: 0;
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
            PathAttribute { name: "recipeOpacity"; value: 1 }
            PathAttribute { name: "recipeZ"; value: 2 }
            PathQuad { x: screen.width/2; y: screen.height/2; controlX: screen.width+(screen.width*2.77); controlY: screen.height/2 }
            PathAttribute { name: "recipeOpacity"; value: 0 }
            PathAttribute { name: "recipeZ"; value: 0 }
            PathQuad { x: screen.width/2; y: screen.height/2; controlX: -screen.width*2.77; controlY: screen.height/2 }
        }

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
                name: "hideOutput_showFinder"
            },
            State {
                name: "showTorrentInformation"
                when: showTorrentInformation
            },
            State {
                name: "hideTorrentInformation"
            },
            State {
                name: "showHelp"
            },
            State {
                name: "hideHelp"
                when: root.hideHelp
            }
        ]
        transitions: [
            Transition {
                to: "show"
                NumberAnimation { target: output; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 500; easing.type: Easing.OutQuart }
            },
            Transition {
                to: "hide"
                SequentialAnimation {
                    NumberAnimation { duration: 20 }
                    NumberAnimation { target: output; easing.amplitude: 1.7; properties: "x"; to: screen.width; duration: 500; easing.type: Easing.OutQuart }
                }
            },
            Transition {
                to: "hideOutput_showFinder"
                SequentialAnimation {
                    PropertyAction { target: root; property: "stopPreview"; value: true }
                    NumberAnimation { duration: 20 }
                    NumberAnimation { target: output; easing.amplitude: 1.7; properties: "x"; to: screen.width; duration: 500; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" } //
                    PropertyAction { target: root_loader_B; property: "source"; value: "Finder.qml" }
                }
            },
            Transition {
                to: "showTorrentInformation"
                PropertyAction { target: root; property: "screenWidth"; value: screen.width }
                PropertyAction { target: output; property: "showTorrentInformation"; value: false }
                PropertyAction { target: handler; property: "enabled"; value: false }
                PropertyAction { target: root_loader_A; property: "source"; value: "TorrentInformation.qml" }
                SequentialAnimation {
                    NumberAnimation { duration: 20 }
                    NumberAnimation { target: output; easing.amplitude: 1.7; properties: "x"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root; property: "stopPreview"; value: true }
                }
            },
            Transition {
                to: "hideTorrentInformation"
                SequentialAnimation {
                    PropertyAction { target: output; property: "x"; value: -root.screenWidth }
                    NumberAnimation { target: output; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1080; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root; property: "stopPreview"; value: false }
                }
            },
            Transition {
                to: "showHelp"
                NumberAnimation { target: output; easing.amplitude: 1.7; properties: "x"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
            },
            Transition {
                to: "hideHelp"
                NumberAnimation { target: output; easing.amplitude: 1.7; properties: "x"; to: 0; duration: 1080; easing.type: Easing.OutQuart }
            }
        ]

        function setFocus() { handler.forceActiveFocus() }

        Keys.onPressed: {
            if (event.key === Qt.Key_Escape) {
                handler.state = "hideOutput_showFinder"
                event.accepted = true;
            } else if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier) && !timeOutNetwork.running) {
                handler.state = "showHelp"
                screen.state = "showHelp"
                event.accepted = true
            } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                root.stopPreview = true
                root.quit_qml_signal()
                event.accepted = true;
            }
        }

        Keys.onUpPressed: {
            if(totalFilms > 5 && counter < totalFilms) {
                handler.enabled = false
                offset = offset+5
                counter = counter+5
                currentFilm = counter-4
                listUpdater(offset)
            }
        }
        Keys.onDownPressed: {
            if(totalFilms > 5 && currentFilm-5 > 0) {
                handler.enabled = false
                offset = offset-5
                counter = counter-5
                currentFilm = counter-4
                listUpdater(offset)
            }
        }
        Keys.onRightPressed: {
            handler.enabled = false
            enabledDelay.start()
            incrementCurrentIndex()
            if (locationOnList === list.length-12) {
                locationOnList = 0
                location = 0
                currentFilm = currentFilm-n+1
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
                locationOnList = list.length-12
                location = n-1
                currentFilm = currentFilm+n-1
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

    Timer {
        id: networkDelay
        running: false
        repeat: false
        interval: 500
        onTriggered: { root_loader_A.source = "Network.qml" }
    }

    function listCreator() {
        var torrent
        var row = 0
        for(var i=0; i<n; i++) {
           torrent = list[row+11].split(",")
           model.append({ "title": list[row], "cast": list[row+1], "categories": list[row+2], "quality": list[row+3], "full": list[row+4],
                          "hostPreview": list[row+5], "urlPreview": list[row+6], "filePreview": list[row+7], "idPreview": i, "hostPosterCover": list[row+8],
                          "urlPoster": list[row+9], "urlCover": list[row+10], "magnet": torrent[0], "scenes": Number(torrent[1]) })
           row += 12
        }
        location = 0
        locationOnList = 0
    }

    function listUpdater(offset) {
        root.stopPreview = true
        handler.state = "hide"
        successPoster = false
        successCover = false
        root.list = ""
        model.clear()
        timeOutNetwork.start()
        networkDelay.start()
        root.find_qml_signal(root.input, root.pornstar, root.category, root.quality, root.full, offset, false)
    }

    Connections {
        target: cppSignals
        onSuccess_update_list_signal: {
            root.stopPreview = false
            root.n = n
            root.list = list
            listCreator()
        }
        onHide_torrent_information_signal: {
            handler.state = "hideTorrentInformation"
            handler.enabled = true
            handler.setFocus()
        }
    }

    Component.onCompleted: {
        listCreator(root.n, root.list)
        timeOutNetwork.start()
    }
}
