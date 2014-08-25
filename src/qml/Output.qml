import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    id: output
    opacity: 0

    property bool posterLoaded
    property bool coverLoaded
    property bool screensLoaded
    property bool abortTorrent
    property bool fileNotReady
    property bool hideAll
    property bool focusPath
    property bool zoomIn
    property bool onShowPlayerLayer

    property int location: 0
    property int currentFilm: 1
    property int locationOnList
    property int nIndex: 0
    property int timeLeft: 5

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
            opacity: { PathView.recipeOpacity || 1 }
            z: { PathView.recipeZ || 1 }

            Keys.onUpPressed: { if (!output.zoomIn && !onShowPlayerLayer) fiveMoreTimer.start() }
            Timer {
                id: fiveMoreTimer
                running: false
                repeat: false
                interval: 500
                onTriggered: { pathView.fiveMore() }
            }
            Keys.onDownPressed: { if (!output.zoomIn && !onShowPlayerLayer) fiveLessTimer.start() }
            Timer {
                id: fiveLessTimer
                running: false
                repeat: false
                interval: 500
                onTriggered: { pathView.fiveLess() }
            }
            Keys.onRightPressed: { if (!output.zoomIn && !onShowPlayerLayer) pathView.next() }
            Keys.onLeftPressed: { if (!output.zoomIn && !onShowPlayerLayer) pathView.prior() }

            Image {
                id: poster
                width: parent.width
                height: parent.height
                source: urlPoster
                sourceSize.width: 1920
                sourceSize.height: 1080
                Rectangle {
                    id: posterLayer
                    color: Qt.rgba(0, 0, 0, 0.6)
                    anchors.fill: parent
                }
                onStatusChanged: { 
                    if (poster.source == list[5] && poster.progress == 1.0) {
                        posterLoaded = true
                        if (!coverLoaded || !screensLoaded)
                            imagesStatus.start()
                    } else if (poster.source == list[5] && poster.status == Image.Error) {
                        poster.source = "qrc:/images/available/posterNotAvailable.png"
                        posterLoaded = true
                        if (!coverLoaded || !screensLoaded)
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
                font.letterSpacing: -10
                font.pixelSize: parent.height/6.35
                anchors.left: parent.left
                anchors.bottom: cover.top
                anchors.bottomMargin: -parent.height/83.07
            }

            Image {
                id: frame
                width: parent.width
                height: parent.height/2.16
                source: "qrc:/images/output/frame.png"
                sourceSize.width: 1920
                sourceSize.height: 500
                anchors.centerIn: parent
            }

            property bool enter
            Image {
                id: screens
                width: screen.width/3.04
                height: screen.height/2.51
                source: urlScreens
                sourceSize.width: 1920
                sourceSize.height: 1080
                opacity: 0.1
                smooth: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: parent.height/540
                onStatusChanged: {
                    if (screens.source == list[7] && poster.progress == 1.0) {
                        screensLoaded = true
                        if (!posterLoaded || !coverLoaded)
                            imagesStatus.start()
                    } else if (screens.source == list[7] && screens.status == Image.Error) {
                        screens.source = "qrc:/images/available/screensNotAvailable.png"
                        screensLoaded = true
                        if (!posterLoaded || !coverLoaded)
                            imagesStatus.start()
                    }
                }
                MouseArea {
                    id: zoom
                    hoverEnabled: true
                    onEntered: { enter = true }
                    onHoveredChanged: { enter = false }
                    anchors.fill: parent
                }
                Keys.onPressed: { if (output.zoomIn && event.key === Qt.Key_Escape) enter = false }
            }
            onEnterChanged: {
                if (enter) {
                    inScreensA.start()
                    inScreensB.start()
                    inScreensC.start()
                    inScreensD.start()
                    output.zoomIn = true
                    zoomInDelay.start()
                } else {
                    inScreensA.stop()
                    inScreensB.stop()
                    inScreensC.stop()
                    inScreensD.stop()
                    outScreensA.start()
                    outScreensB.start()
                    outScreensC.start()
                    outScreensD.start()
                    zoomOutDelay.start()
                }
            }
            Timer {
                id: zoomInDelay
                running: false
                repeat: false
                interval: 800
                onTriggered: { screens.forceActiveFocus() }
            }
            Timer {
                id: zoomOutDelay
                running: false
                repeat: false
                interval: 1000
                onTriggered: { recipe.forceActiveFocus(); output.zoomIn = false }
            }
            NumberAnimation { id: inScreensA; target: screens; properties: "z"; to: 5; duration: 500; easing.type: Easing.InOutQuart }
            NumberAnimation { id: inScreensB; target: screens; properties: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuart }
            NumberAnimation { id: inScreensC; target: screens; properties: "width"; to: parent.width; duration: 1000; easing.type: Easing.InOutQuart }
            NumberAnimation { id: inScreensD; target: screens; properties: "height"; to: parent.height; duration: 1000; easing.type: Easing.InOutQuart }
            NumberAnimation { id: outScreensA; target: screens; properties: "z"; to: 1; duration: 1000; easing.type: Easing.InOutQuart }
            NumberAnimation { id: outScreensB; target: screens; properties: "width"; to: parent.width/3.04; duration: 1000; easing.type: Easing.InOutQuart }
            NumberAnimation { id: outScreensC; target: screens; properties: "height"; to: parent.height/2.51; duration: 1000; easing.type: Easing.InOutQuart }
            NumberAnimation { id: outScreensD; target: screens; properties: "opacity"; to: 0.1; duration: 1000; easing.type: Easing.InOutQuart }
            
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
                source: urlCover
                sourceSize.width: 415
                sourceSize.height: 600
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -parent.width/15.86
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -parent.height/270
                onStatusChanged: {
                    if (cover.source == list[6] && cover.progress == 1.0) {
                        coverLoaded = true
                        if (!posterLoaded || !screensLoaded)
                            imagesStatus.start()
                    } else if (cover.source == list[6] && cover.status == Image.Error) {
                        cover.source = "qrc:/images/available/coverNotAvailable.png"
                        coverLoaded = true
                        if (!posterLoaded || !screensLoaded)
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
                        loader.source = ""
                        recipe.state = "showAll"
                        focusDelay.start()
                    } else {
                        imagesStatus.restart()
                    }
                }
            }
            Column {
                id: datesColumn
                spacing: parent.height/216
                anchors.left: cover.right
                anchors.leftMargin: parent.width/54.85
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -parent.height/15.88
                Text {
                    id: castLabel
                    text: cast
                    color: "white"
                    font.family: pigFont.name
                    font.italic: true
                    font.bold: true
                    font.pixelSize: screen.height/36
                }
                Text {
                    id: categoryLabel
                    text: categories
                    color: "white"
                    font.family: pigFont.name
                    font.italic: true
                    font.bold: true
                    font.pixelSize: screen.height/36
                }
                Text {
                    id: fullLabel
                    text: "FULL"
                    color: { if (full === "NOT") "black"; else "white" }
                    font.family: pigFont.name
                    font.italic: true
                    font.bold: true
                    font.strikeout: { if (full === "NOT") true; else false }
                    font.pixelSize: screen.height/36
                }
            }
            Row {
                id: openScenneRow
                spacing: 1
                anchors.left: datesColumn.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: parent.height/20.37
                Component.onCompleted: {
                    for (var i=1; i<=scennes && i<=8; i++) {
                        var component = Qt.createComponent("ButtonScene.qml")
                        var object = component.createObject(openScenneRow)
                        object.quality = quality
                        object.magnet = magnet
                        object.scene = i
                    }
                }
            }
            Text {
                id: noSplitLabel
                text: "UNSPLIT"
                color: "black"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: parent.height/54
                visible: { if (scennes === 1) true; else false }
                anchors.left: openScenneRow.right
                anchors.leftMargin: parent.width/960
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: parent.height/16.95
            }
            Row {
                id: counterRow
                spacing: parent.width/960
                anchors.right: parent.right
                anchors.rightMargin: parent.width/192
                anchors.bottom: parent.bottom
                Text {
                    id: currentLabel
                    text: currentFilm
                    color: Qt.rgba(1, 1, 1, 1)
                    font.family: pigFont.name
                    font.pixelSize: screen.height/27.69
                }
                Text {
                    id: totalLabel
                    text: totalFilms
                    color: Qt.rgba(1, 1, 1, 0.5)
                    font.family: pigFont.name
                    font.pixelSize: screen.height/27.69
                }
            }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: poster; property: "opacity"; to: 1; duration: 4000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: translucedLayer; property: "opacity"; to: 0.7; duration: 4000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 1; duration: 2000; easing.type: Easing.OutElastic }
            PropertyAction  { running: !recipe.PathView.isCurrentItem; target: poster; property: "opacity"; value: 0 }
            PropertyAction  { running: !recipe.PathView.isCurrentItem; target: translucedLayer; property: "opacity"; value: 0 }
            NumberAnimation { running: !recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 0; duration: 1; easing.type: Easing.InOutQuad }

            states: [
                State {
                    name: "showPlayerLayer"
                    when: onShowPlayerLayer
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
                    to: "showAll"
                    NumberAnimation { target: output; property: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuad }
                },
                Transition {
                    to: "hideAll"
                    NumberAnimation { target: output; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
                }
            ]            
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
            if (locationOnList === list.length-9) {
                locationOnList = 0
                location = 0
                currentFilm = currentFilm-n+1 
            } else {
                locationOnList = locationOnList+9
                ++currentFilm
                ++location
            }
            focusDelay.start()
        }
        onPrior: {
            focusPath = false
            decrementCurrentIndex()
            if (locationOnList === 0) {
                locationOnList = list.length-9
                location = n-1
                currentFilm = currentFilm+n-1 
            } else {
                locationOnList = locationOnList-9
                --currentFilm
                --location
            }
            focusDelay.start()
        }
        Timer {
            id: focusDelay
            running: false
            repeat: false
            interval: 200
            onTriggered: { focusPath = true }
        }

        Keys.onPressed: {
            if (!onShowPlayerLayer) {
                if (!zoomIn && event.key === Qt.Key_Escape) {
                    loader_finder_output.source = "Finder.qml"
                    event.accepted = true;
                } else if (event.key === Qt.Key_H && (event.modifiers & Qt.ControlModifier)) {
                    screen.state = "showHelp"
                    event.accepted = true
                } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
                    root.quitSIGNAL_QML()
                    event.accepted = true;
                }
            }
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

        Button {
            id: abortTorrentButton
            width: screen.width/19.2 //100
            height: screen.height/43.2 //25
            label: "ABORT"
            labelColor: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            labelInColor: "white"
            labelOutColor: Qt.rgba(0.1, 0.1, 0.1, 0.5)
            labelBold: true
            labelSize: screen.height/36 //30
            visible:  { if (playerLayer.height === screen.height && !abortTorrent) true; else false }
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -screen.width/10.66 //-180
            anchors.verticalCenterOffset: screen.height/600 //1.8
            onClicked: { abortTorrent = true }
        }
        NumberAnimation { id: hidePlayerLayer; running: false; target: playerLayer; property: "height"; to: 0; duration: 1000; easing.type: Easing.InOutQuart }
        Timer {
            id: abortTorrentResetDelay
            running: false
            repeat: false
            interval: 1100
            onTriggered: {
                root.peers = 0
                root.required = 0
                root.bitRate = ""
                onShowPlayerLayer = false
                abortTorrent = false
                fileNotReady = false
            }
        }
        ProgressBar {
            id: progressBar
            value: root.downloaded
            visible: { if (playerLayer.height > 24 && !abortTorrent) true; else false }
            anchors.centerIn: parent
        }
        Row {
            id: peerInformationRow
            spacing: screen.width/112.94 //17
            visible: { !abortTorrent }
            anchors.left: progressBar.right
            anchors.leftMargin: screen.width/96 //20
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: bitRateLabel
                text: {
                    if (root.bitRate !== 0 && playerLayer.height === screen.height)
                        root.bitRate+"kb/s"
                    else if (playerLayer.height === screen.height)
                        "CONNECTING"
                    else
                        ""
                }
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36 //30
            }
            Text {
                id: peersLabel
                text: {
                    if (root.peers !== 0 && playerLayer.height === screen.height)
                        "PEERS "+root.peers
                    else
                        ""
                }
                color: "white"
                font.family: pigFont.name
                font.bold: true
                font.pixelSize: screen.height/36 //30
            }
        }
        Text {
            id: checkingFileLabel
            text: "CHECKING FILE"
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/72 //15
            visible: { fileNotReady && !abortTorrent }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: progressBar.bottom
            anchors.topMargin: screen.height/108 //10
        }
        Text {
            id: fileRecheckLabel
            text: ""
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: pigFont.name
            font.pixelSize: screen.height/72 //15
            visible: { fileNotReady && !abortTorrent }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: checkingFileLabel.bottom
            anchors.topMargin: screen.height/540 //2
        }
        Timer {
            id: fileRecheckDelay
            running: false
            repeat: true
            interval: 1000
            onTriggered: {
                if (output.timeLeft > 0)
                    output.timeLeft -= 1
                else
                    output.timeLeft = 5
                checkingFileLabel.text = "FILE NOT READY. PLEASE WAIT"
                fileRecheckLabel.text = "RECHECK ON "+output.timeLeft
            }
        }
    }
    onAbortTorrentChanged: {
        if (abortTorrent) {
            root.torrentHandleSIGNAL_QML("", "", true)
            root.downloaded = 0
            hidePlayerLayer.running = true
            abortTorrentResetDelay.start()
        }
    }
    
    function listCreator() {
        var torrent
        var row = 0
        for(var i=0; i<n; i++) {
           torrent = list[row+8].split(",")
           var nScennes = Number(torrent[1])
           model.append({ "title": list[row], "cast": list[row+1], "categories": list[row+2], "quality": list[row+3], "full": list[row+4],
                          "urlPoster": list[row+5], "urlCover": list[row+6], "urlScreens": list[row+7], "magnet": torrent[0], "scennes": nScennes })
           row += 9
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
        root.findSIGNAL_QML(root.input, root.pornstar, root.category, root.quality, root.full, offset, false)
        waitDelay.start();
    }
    Timer {
        id: waitDelay
        running: false
        repeat: false
        interval: 600
        onTriggered: { loader.source = "Wait.qml" }
    }
    
    Connections {
        target: cppSignals
        onListUpdatedSIGNAL: { listCreator() }
        onAbortTorrentSIGNAL: { abortTorrent = true }
        onCheckingFileSIGNAL: { fileNotReady = true; output.timeLeft = 5; fileRecheckDelay.start() }
        onFileReadySIGNAL: { fileNotReady = false; fileRecheckDelay.stop() }
    }

    Component.onCompleted: { listCreator(root.n, root.list) }
}
