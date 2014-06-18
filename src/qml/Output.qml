import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    id: output
    opacity: 0

    property bool posterLoaded
    property bool coverLoaded
    property bool hideAll
    property bool focusPath

    property string inputText
    property string category
    property string pornstar

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
            width: listView.width
            height: listView.height
            opacity: PathView.recipeOpacity || 1
            z: PathView.recipeZ || 1

            Keys.onUpPressed: { fiveMoreTimer.start() } //{ previewPlayer.kill(); fiveMoreTimer.start() }
            Timer {
                id: fiveMoreTimer
                running: false
                repeat: false
                interval: 500
                onTriggered: pathView.fiveMore()
            }
            Keys.onDownPressed: { fiveLessTimer.start() } //{ previewPlayer.kill(); fiveLessTimer.start() }
            Timer {
                id: fiveLessTimer
                running: false
                repeat: false
                interval: 500
                onTriggered: pathView.fiveLess()
            }
            Keys.onRightPressed: { pathView.next() } //{ pathView.next(); previewPlayerKillTimer.start() }
            Keys.onLeftPressed: { pathView.prior() } //{ pathView.prior(); previewPlayerKillTimer.start() }
            Timer {
                id: previewPlayerKillTimer
                running: false
                repeat: false
                interval: 1000
                //onTriggered: previewPlayer.kill()
            }

            Image {
                id: posters
                source: urlPoster
                fillMode: Image.PreserveAspectCrop
                Rectangle {
                    id: postersLayer
                    color: Qt.rgba(0, 0, 0, 0.6)
                    anchors.fill: parent
                }
                onStatusChanged: { 
                    if (posters.source == list[5] && posters.progress == 1.0) {
                        posterLoaded = true
                        if (!coverLoaded)
                            imagesStatus.start()
                    } else if (posters.source == list[5] && posters.status == Image.Error) {
                        // TODO: Cargar imagen de 'Imagen no disponible'.
                        posterLoaded = true
                        if (!coverLoaded)
                            imagesStatus.start()
                    }
                }
            }
            GaussianBlur {
                id: posterEffect
                source: posters
                radius: 30
                samples: 32
                anchors.fill: posters
            }
            Image {
                id: stripes
                source: "qrc:/images/stripes.png"
                fillMode: Image.PreserveAspectCrop
            }

            RectangularGlow {
                id: coverEffect
                color: "black"
                glowRadius: 35
                spread: 0.2
                cornerRadius: 40
                anchors.fill: cover
            }
            Text {
                id: titleLabel
                text: title
                color: "white"
                font.family: pigFont.name
                font.letterSpacing: -10
                font.pixelSize: 225/strap
                anchors.left: parent.left
                anchors.bottom: cover.top
                anchors.bottomMargin: -41
            }
            Image {
                id: frame
                width: screen.width
                height: 500
                sourceSize.width: screen.width
                sourceSize.height: 500
                source: "qrc:/images/frame.png"
                anchors.centerIn: parent
            }
            Image {
                id: cover
                width: 415
                height: 600
                sourceSize.width: 415
                sourceSize.height: 600
                source: urlCover
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -121
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -4
                onStatusChanged: {
                    if (cover.source == list[4] && cover.progress == 1.0) {
                        coverLoaded = true
                        if (!posterLoaded)
                            imagesStatus.start()
                    } else if (cover.source == list[4] && cover.status == Image.Error) {
                        // TODO: Cargar imagen de 'Imagen no disponible'.
                        coverLoaded = true
                        if (!posterLoaded)
                            imagesStatus.start()
                    }
                }
            }
            Timer {
                id: imagesStatus
                running: false
                repeat: false
                interval: 20
                onTriggered: {
                    if (posterLoaded && coverLoaded) {
                        loaderWaitMsg.active = false
                        recipe.state = "showAll"
                        delayFocus.start()
                    }else {
                        imagesStatus.restart()
                    }
                }
            }
            Column {
                id: datesColumn
                spacing: 5
                anchors.left: cover.right
                anchors.leftMargin: 25
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -68
                Text {
                    id: atressesLabel
                    text: actresses
                    color: "white"
                    font.family: pigLightFont.name
                    font.italic: true
                    font.bold: true
                    font.pixelSize: 30
                }
                Text {
                    id: categoryLabel
                    text: categories
                    color: "white"
                    font.family: pigLightFont.name
                    font.italic: true
                    font.bold: true
                    font.pixelSize: 30
                }
            }
            Row {
                id: openScenneRow
                spacing: 1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: 310
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 31

                ButtonScenne {
                    id: openScenne1
                    iconOpacity: { if (scenneId1 != '') 1; else 0.15 }
                    iconQuality: {
                        if (scenneId1 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (scenneId1 != '') {
                            //previewPlayer.kill()
                            recipe.state = "showPlayerLayer"
                            root.torrentHandle(magnetUrl, scenneId1)
                        }
                    }
                }
                ButtonScenne {
                    id: openScenne2
                    iconOpacity: { if (scenneId2 != '') 1; else 0.15 }
                    iconQuality:  {
                        if (scenneId2 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (scenneId2 != '') {
                            //previewPlayer.kill()
                            recipe.state = "showPlayerLayer"
                            root.torrentHandle(magnetUrl, scenneId2)
                        }
                    }
                }
                ButtonScenne {
                    id: openScenne3
                    iconOpacity: { if (scenneId3 != '') 1; else 0.15 }
                    iconQuality: {
                        if (scenneId3 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (scenneId3 != '') {
                            //previewPlayer.kill()
                            recipe.state = "showPlayerLayer"
                            root.torrentHandle(magnetUrl, scenneId3)
                        }
                    }
                }
                ButtonScenne {
                    id: openScenne4
                    iconOpacity: { if (scenneId4 != '') 1; else 0.15 }
                    iconQuality: {
                        if (scenneId4 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (scenneId4 != '') {
                            //previewPlayer.kill()
                            recipe.state = "showPlayerLayer"
                            root.torrentHandle(magnetUrl, scenneId4)
                        }
                    }
                }
            }

            Text {
                id: counterLabel
                text: currentFilm+" » "+totalFilms
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: 35
                visible: false//
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            /*
            PreviewPlayer {
                id: previewPlayer
                previewWidth: 635    // TODO: Pasar este parametro como screen.width/?
                previewHeight: 432.4 // TODO: Pasarle este parametro como screen.height/?
                url: urlPreview
            }
            */

            NumberAnimation { running: recipe.PathView.isCurrentItem; target: posters; property: "opacity"; to: 1; duration: 4000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: posterEffect; property: "opacity"; to: 1; duration: 4000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 1; duration: 2000; easing.type: Easing.OutElastic }
            PropertyAction  { running: !recipe.PathView.isCurrentItem; target: posters; property: "opacity"; value: 0 }
            PropertyAction  { running: !recipe.PathView.isCurrentItem; target: posterEffect; property: "opacity"; value: 0 }
            NumberAnimation { running: !recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 0; duration: 1; easing.type: Easing.InOutQuad }

            states: [
                State {
                    name: "showPlayerLayer"
                    PropertyChanges { target: openScenne1; iconOpacity: "0.15" }
                    PropertyChanges { target: openScenne2; iconOpacity: "0.15" }
                    PropertyChanges { target: openScenne3; iconOpacity: "0.15" }
                    PropertyChanges { target: openScenne4; iconOpacity: "0.15" }
                },
                State {
                    name: "hidePlayerLayer"
                },
                State {
                    name: "showAll"
                },
                State {
                    name: "hideAll"
                    when: hideAll
                }
            ]
            transitions: [
                Transition {
                    to: "showPlayerLayer"
                        NumberAnimation { target: playerLayer; property: "height"; to: screen.height; duration: 2000; easing.type: Easing.InOutQuart }
                },
                Transition {
                    to: "hidePlayerLayer"
                        NumberAnimation { target: playerLayer; property: "height"; to: 0; duration: 1000; easing.type: Easing.InOutQuart }
                        PropertyAction { target: openScenne1; property: "iconOpacity"; value: "1" }
                        PropertyAction { target: openScenne2; property: "iconOpacity"; value: "1" }
                        PropertyAction { target: openScenne3; property: "iconOpacity"; value: "1" }
                        PropertyAction { target: openScenne4; property: "iconOpacity"; value: "1" }
                },
                Transition {
                    to: "showAll"
                    NumberAnimation { target: output; property: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuad }
                },
                Transition {
                    to: "hideAll"
                    NumberAnimation { target: output; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
                }
            ]
            
            Connections {
                target: cppSignals
                onHidePlayerLayerSIGNAL: { recipe.state = "hidePlayerLayer" }
            }
        }
    }

    PathView {
        id: pathView
        model: model
        delegate: delegate
        interactive: false
        anchors.fill: parent
        
        property int offset: 0
        property int counter: 5

        signal fiveMore()
        signal fiveLess()
        signal next()
        signal prior()
        
        onFiveMore: {  
            if(totalFilms > 5 && counter < totalFilms) {
                offset = offset+5
                counter = counter+5
                currentFilm = counter-4
                listUpdater(offset)
            }
        }
        onFiveLess: {
            if(totalFilms > 5 && currentFilm-4 > 0) { // FIXME: Si esta en el primero no deberia aceptar ir para abajo.
                offset = offset-5                     // FIXME: Se rompe si voy hacia arriba o hacia abajo rapido, creo que no llega a crear y destruir los objetos.
                counter = counter-5                   
                currentFilm = counter-4
                listUpdater(offset)
            }
        }
        onNext: {
            focusPath = false
            incrementCurrentIndex()
            if (locationOnList == list.length-11) {
                locationOnList = 0
                location = 0
                currentFilm = currentFilm-n+1 
            } else {
                locationOnList = locationOnList+11
                ++currentFilm
                ++location
            }
            delayFocus.start()
        }
        onPrior: {
            focusPath = false
            decrementCurrentIndex()
            if (locationOnList == 0) {
                locationOnList = list.length-11
                location = n-1
                currentFilm = currentFilm+n-1 
            } else {
                locationOnList = locationOnList-11
                --currentFilm
                --location
            }
            delayFocus.start()
        }
        Timer {
            id: delayFocus
            running: false
            repeat: false
            interval: 200
            onTriggered: focusPath = true
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_Escape && !(event.modifiers & Qt.ShiftModifier)) {  
                back()
                event.accepted = true;
            } else if (event.key === Qt.Key_Escape && (event.modifiers & Qt.ShiftModifier)) {  
                root.quit()
                event.accepted = true;
            }
        }
        function back() {
            currentFilm = 1
            focusPath = false
            loaderFinder.source = "qrc:/src/qml/Finder.qml"
            loaderFinder.active = true
        }

        path: Path {
            startX: parent.width/2
            startY: parent.height/2
            PathAttribute { name: "recipeOpacity"; value: 1 }
            PathAttribute { name: "recipeZ"; value: 2 }
            PathQuad { x: parent.width/2; y: parent.height/2; controlX: parent.width+parent.width*2.77; controlY: parent.height/2 }
            PathAttribute { name: "recipeOpacity"; value: 0 }
            PathAttribute { name: "recipeZ"; value: 0 }
            PathQuad { x: parent.width/2; y: parent.height/2; controlX: -parent.width*2.77; controlY: parent.height/2 }
        }
    }

    Rectangle {
        id: playerLayer
        width: screen.width
        color: "black"

        ProgressBar {
           id: progressBar
           value: { if (root.downloadedPieces == 0 && root.bitRate != 0) 1; else root.downloadedPieces }
           visible: { if (playerLayer.height > 24) true; else false }
           anchors.centerIn: parent
        }
        Text {
            id: bitRateLabel
            text: {
                if (root.bitRate != 0 && playerLayer.height == screen.height)
                    root.bitRate+"kb/s"
                else if (playerLayer.height == screen.height)
                    "CONNECTING"
                else
                    ""
            }
            color: "white"
            font.family: pigFont.name
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: { if (bitRateLabel.text == "CONNECTING") 225; else 190 }
        }
    }

    Text {
        id: keysInfo
        text: "⌨"
        color: Qt.rgba(1, 1, 1, 0.15)
        font.family: "Verdana"
        font.pixelSize: 30
        visible: { if (playerLayer.height < screen.height -50) true; else false }
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -2 
        MouseArea {
            hoverEnabled: true
            onEntered: delayIn.restart()
            onHoveredChanged: delayOut.restart()
            anchors.fill: parent
        }
        Timer {
            id: delayIn
            running: false
            repeat: false
            interval: 25
            onTriggered: keymap.visible = true
        }
        Timer {
            id: delayOut
            running: false
            repeat: false
            interval: 20
            onTriggered: keymap.visible = false
        }
    }
    Image {
        id: keymap
        source: "qrc:/images/keymap.png"
        fillMode: Image.PreserveAspectCrop
        visible: false
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
    }
    
    function listCreator() {
        inputText = list[8]
        category = list[9]
        pornstar = list[10]
        var torrent
        var row = 0
        for(var i=0; i<n; i++) {
           torrent = list[row+7].split(",")
           model.append({ "title": list[row], "actresses": list[row+1], "quality": list[row+2], "categories": list[row+3],
                          "urlCover": list[row+4], "urlPoster": list[row+5], "urlPreview": list[row+6], "magnetUrl": torrent[0],
                          "scenneId1": torrent[1], "scenneId2": torrent[2], "scenneId3": torrent[3], "scenneId4": torrent[4] })
           row +=11
        }
        location = 0
        locationOnList = 0
        pathView.forceActiveFocus()
    }
    
    function listUpdater(offset) {
        hideAll = true
        focusPath = false
        posterLoaded = false
        coverLoaded = false
        root.list = ''
        model.clear()
        loaderWaitMsg.active = true
        root.findDb(inputText, category, pornstar, offset, false)
    }
    
    Connections {
        target: cppSignals

        onListUpdatedSIGNAL: { listCreator() }
    }

    Component.onCompleted: { listCreator(root.n, root.list) }
}
