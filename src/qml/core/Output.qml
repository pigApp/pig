import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: output
    opacity: 0

    property bool posterLoaded
    property bool coverLoaded
    property bool hideScreens
    property bool hideAll
    property string inputText
    property string category
    property string pornstar
    property string videoID
    property bool focusPath
    property int location: 0
    property int currentFilm: 1
    property int locationOnList
    property int nIndex: 0

    property bool run: root.listUpdated
    
    onRunChanged: { listCreator() }
    Component.onCompleted: { listCreator(root.n, root.list) }

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

            Keys.onUpPressed: { previewPlayer.kill(); fiveMoreTimer.start() }
            Timer {
                id: fiveMoreTimer
                running: false
                repeat: false
                interval: 500
                onTriggered: pathView.fiveMore()
            }
            Keys.onDownPressed: { previewPlayer.kill(); fiveLessTimer.start() }
            Timer {
                id: fiveLessTimer
                running: false
                repeat: false
                interval: 500
                onTriggered: pathView.fiveLess()
            }
            Keys.onRightPressed: { pathView.next(); previewPlayerKillTimer.start() }
            Keys.onLeftPressed: { pathView.prior(); previewPlayerKillTimer.start() }
            Timer {
                id: previewPlayerKillTimer
                running: false
                repeat: false
                interval: 1000
                onTriggered: previewPlayer.kill()
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
                    if (posters.source == list[6] && posters.progress == 1.0) {
                        posterLoaded = true
                        if (!coverLoaded)
                            imagesStatus.start()
                    } else if (posters.source == list[6] && posters.status == Image.Error) {
                        //cargar imagen de 'Imagen no disponible'
                        posterLoaded = true
                        if (!coverLoaded)
                            imagesStatus.start()
                    }
                }
            }
            GaussianBlur {
                id: posterEffect
                source: posters
                radius: 20
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
                color: "white"
                glowRadius: 20
                spread: 0.2
                cornerRadius: 50
                anchors.fill: cover
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
                width: 318
                height: 432
                sourceSize.width: 318
                sourceSize.height: 432
                source: urlCover
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -165
                anchors.verticalCenterOffset: 2
                onStatusChanged: {
                    if (cover.source == list[5] && cover.progress == 1.0) {
                        coverLoaded = true
                        if (!posterLoaded)
                            imagesStatus.start()
                    } else if (cover.source == list[5] && cover.status == Image.Error) {
                        //cargar imagen de 'Imagen no disponible'
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
                    if (posterLoaded && coverLoaded && transition) {
                        root.showWaitSpinner = false
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
                anchors.leftMargin: 20
                anchors.verticalCenter: frame.verticalCenter
                Text {
                    id: atressesLabel
                    text: actresses
                    color: "white"
                    font.family: pigLightFont.name
                    font.bold: true
                    font.pixelSize: 25
                }
                Text {
                    id: categoryLabel
                    text: categories
                    color: "white"
                    font.family: pigLightFont.name
                    font.bold: true
                    font.pixelSize: 25
                }
                Text {
                    id: collaboratorLabel
                    text: collaborator
                    color: "white"
                    font.family: pigLightFont.name
                    font.bold: true
                    font.pixelSize: 25
                }
            }
            Row {
                id: openScenneRow
                spacing: 15
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: screen.width/3
                ButtonScenne {
                    id: openScenne1
                    iconOpacity: { if (urlScenne1 != '') 1; else 0.15 }
                    iconQuality: {
                        if (urlScenne1 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (urlScenne1 != '') {
                            previewPlayer.kill()
                            videoID = urlScenne1
                            recipe.state = "showPlayerLayer"
                            delayPlayerLayer.start()
                        }
                    }
                }
                ButtonScenne {
                    id: openScenne2
                    iconOpacity: { if (urlScenne2 != '') 1; else 0.15 }
                    iconQuality:  {
                        if (urlScenne2 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (urlScenne2 != '') {
                            previewPlayer.kill()
                            videoID = urlScenne2
                            recipe.state = "showPlayerLayer"
                            delayPlayerLayer.start()
                        }
                    }
                }
                ButtonScenne {
                    id: openScenne3
                    iconOpacity: { if (urlScenne3 != '') 1; else 0.15 }
                    iconQuality: {
                        if (urlScenne3 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (urlScenne3 != '') {
                            previewPlayer.kill()
                            videoID = urlScenne3
                            recipe.state = "showPlayerLayer"
                            delayPlayerLayer.start()
                        }
                    }
                }
                ButtonScenne {
                    id: openScenne4
                    iconOpacity: { if (urlScenne4 != '') 1; else 0.15 }
                    iconQuality: {
                        if (urlScenne4 == '') {
                            "qrc:/images/notAvailable.png"
                        } else {
                            if (quality == '1080p')
                                "qrc:/images/1080.png"
                            else
                                "qrc:/images/720.png"
                        }
                    }
                    onClicked: {
                        if (urlScenne4 != '') {
                            previewPlayer.kill()
                            videoID = urlScenne4
                            recipe.state = "showPlayerLayer"
                            delayPlayerLayer.start()
                        }
                    }
                }
            }
            Text {
                id: counterLabel
                text: currentFilm+" » "+totalFilms 
                color: Qt.rgba(1, 1, 1, 0.15)
                font.family: pigLightFont.name
                font.bold: true
                font.pixelSize: 25
                anchors.right: parent.right
                anchors.rightMargin: 30
                anchors.verticalCenter: parent.verticalCenter
            }
            Timer {
                id: delayPlayerLayer
                running: false
                repeat: false
                interval: 2010
                onTriggered: root.openPlayer(videoID)
            }

            PreviewPlayer {
                id: previewPlayer
                previewWidth: 635 // Pasarle este parametro como screen.width/?
                previewHeight: 432.4 // Pasarle este parametro como screen.height/?
                url: urlPreview
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
                anchors.bottomMargin: -50
            }

            NumberAnimation { running: recipe.PathView.isCurrentItem; target: posters; property: "opacity"; to: 1; duration: 4000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: posterEffect; property: "opacity"; to: 1; duration: 4000; easing.type: Easing.InOutQuad }
            NumberAnimation { running: recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 1; duration: 2000; easing.type: Easing.OutElastic }
            PropertyAction  { running: !recipe.PathView.isCurrentItem; target: posters; property: "opacity"; value: 0 }
            PropertyAction  { running: !recipe.PathView.isCurrentItem; target: posterEffect; property: "opacity"; value: 0 }
            NumberAnimation { running: !recipe.PathView.isCurrentItem; target: datesColumn; property: "opacity"; to: 0; duration: 1; easing.type: Easing.InOutQuad }

            states: [
                State {
                    name: "showPlayerLayer"
                },
                State {
                    name: "hidePlayerLayer"
                    when: playerClosed

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
                        SequentialAnimation {
                            NumberAnimation { target: playerLayer; property: "height"; to: 0; duration: 1000; easing.type: Easing.InOutQuart }
                            PropertyAction { target: root; property: "playerClosed"; value: "false" }
                        }
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
        //focus: focusPath
        interactive: false

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
                currentFilm = counter-4//+1 
                listUpdater(offset)
            }
        }
        onFiveLess: {
            if(totalFilms > 5 && currentFilm-4 > 0) { // Falla si esta en el primero(no deberia aceptar ir para abajo)
                offset = offset-5                     // Se rompe si voy hacia arriba o hacia abajo rapido[creo que no llega a crear y destruir los objetos]
                counter = counter-5                   
                currentFilm = counter-4
                listUpdater(offset)
            }
        }
        onNext: {
            focusPath = false
            incrementCurrentIndex()
            if (locationOnList == list.length-12) {
                locationOnList = 0
                location = 0
                currentFilm = currentFilm-n+1 
            } else {
                locationOnList = locationOnList+12
                ++currentFilm
                ++location
            }
            delayFocus.start()
        }
        onPrior: {
            focusPath = false
            decrementCurrentIndex()
            if (locationOnList == 0) {
                locationOnList = list.length-12
                location = n-1
                currentFilm = currentFilm+n-1 
            } else {
                locationOnList = locationOnList-12
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

        Keys.onEscapePressed: back()
        
        function back() {
            currentFilm = 1
            focusPath = false
            //root.showOutput = false
            //outputCore.state = "back"
            root.sourceFinder = "qrc:/src/qml/core/Finder.qml"
            root.activeFinder = true
            delayCleanModel.start()
        }
        Timer {
            id: delayCleanModel
            repeat: false
            interval: 800
            onTriggered: model.clear()
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
        anchors.fill: parent
    }

    Rectangle {
        id: playerLayer
        width: screen.width
        color: "black"
    }

    Text {
        id: keysInfo
        text: "⌨"
        color: Qt.rgba(1, 1, 1, 0.05)
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
        opacity: 0.3
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
    }

    Text {
         id: dbErrorLabel
         text: status
         color: "white"
         font.pixelSize: 18/strap
         textFormat: Text.RichText
         visible: showDbError
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.bottom: parent.bottom
         anchors.bottomMargin: 60
    }
    Text {
         id: dbErrorInformationLabel
         text: statusInformation
         color: Qt.rgba(0.5, 0.5, 0.5, 1)
         font.pixelSize: 15/strap
         textFormat: Text.RichText
         visible: showDbError
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.bottom: parent.bottom
         anchors.bottomMargin: 20*strap
    }
    
    function listCreator() {
        inputText = list[9]
        category = list[10]
        pornstar = list[11]
        var scenne
        var row = 0
        for(var i=0; i<n; i++) {
           scenne = list[row+8].split(",")
           model.append({ "title": list[row], "actresses": list[row+1], "quality": list[row+2], "collaborator": list[row+3], "categories": list[row+4], "urlCover": list[row+5],
                          "urlPoster": list[row+6], "urlPreview": list[row+7], "urlScenne1": scenne[0], "urlScenne2": scenne[1], "urlScenne3": scenne[2], "urlScenne4": scenne[3] })
           row +=12
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
        showWaitSpinner = true
        root.list = ''
        root.listUpdated = false
        //outputCore.state = ''
        model.clear()
        root.findDb(inputText, category, pornstar, offset, false)
    }
}
