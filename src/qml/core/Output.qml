import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: output

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
                glowRadius: 35
                spread: 0
                cornerRadius: 150
                anchors.fill: cover
            }
            Image {
                id: cover
                width: 677
                height: 435
                sourceSize.width: 677
                sourceSize.height: 435
                source: urlCover
                anchors.centerIn: parent
                anchors.verticalCenterOffset: 1
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
            Image {
                id: frameCentral
                width: 683
                height: 500
                sourceSize.width: 683
                sourceSize.height: 500
                source: "qrc:/images/frameCentral.png"
                anchors.centerIn: parent
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
            /*
            Column {
                id: datesColumn
                spacing: 5
                anchors.centerIn: parent
                anchors.verticalCenterOffset: 325
                Text {
                    id: atressesLabel
                    text: actresses
                    color: Qt.rgba(1, 1, 1, 0.2)
                    font.family: pigFont.name
                    font.pixelSize: 25
                }
                Text {
                    id: qualityLabel
                    text: quality
                    color: Qt.rgba(1, 1, 1, 0.2)
                    font.family: pigFont.name
                    font.pixelSize: 25
                }
                Text {
                    id: collaboratorLabel
                    text: collaborator
                    color: Qt.rgba(1, 1, 1, 0.2)
                    font.family: pigFont.name
                    font.pixelSize: 25
                }
                Text {
                    id: counterLabel
                    text: "FILM "+currentFilm+" FROM A TOTAL "+totalFilms+" FOUND"
                    color: Qt.rgba(1, 1, 1, 0.1)
                    font.family: "Verdana"
                    font.italic: true
                    font.pixelSize: 15
                }
            }*/

            Image {
                id: frameLeft
                width: 713
                height: 500
                sourceSize.width: 713
                sourceSize.height: 500
                source: "qrc:/images/frameLeft.png"
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -695
            }
            PreviewPlayer {
                id: previewPlayer
                previewWidth: 620  // Pasarle este parametro como screen.width/?
                previewHeight: 432.4 // Pasarle este parametro como screen.height/?
                url: urlPreview
            }

            Image {
                id: frameRight
                width: 713
                height: 500
                sourceSize.width: 700
                sourceSize.height: 500
                source: "qrc:/images/frameRight.png"
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: 695
                Row {
                    id: openScenneRow
                    spacing: 15
                    anchors.centerIn: parent
                    anchors.horizontalCenterOffset: -35
                    ButtonScenne {
                        id: openScenne1
                        sceneLabel: { if (urlScenne1 != '') "FULL SCENE"; else 'NOT AVAILABLE' }
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
                        sceneLabel: { if (urlScenne2 != '') "FULL SCENE"; else 'NOT AVAILABLE' }
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
                        sceneLabel: { if (urlScenne3 != '') "FULL SCENE"; else 'NOT AVAILABLE' }
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
                        sceneLabel: { if (urlScenne4 != '') "FULL SCENE"; else 'NOT AVAILABLE' }
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
                Timer {
                    id: delayPlayerLayer
                    running: false
                    repeat: false
                    interval: 2010
                    onTriggered: root.openPlayer(videoID)
                }
            }
            Text {
                id: categoryLabel
                y: 211
                text: categories
                color: Qt.rgba(1, 1, 1, 0.03)
                font.family: pigFont.name
                font.pixelSize: 81/strap
                anchors.horizontalCenter: frameRight.horizontalCenter
                anchors.horizontalCenterOffset: -39
                anchors.top: frameRight.top
                anchors.topMargin: frameRight.height/6
            }

            Text {
                id: titleLabel
                y: 127
                text: title
                color: "white"
                font.family: pigFont.name
                font.letterSpacing: -10
                font.pixelSize: 185/strap
                anchors.left: parent.left
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
                    NumberAnimation { target: outputCore; property: "opacity"; to: 1; duration: 1000; easing.type: Easing.InOutQuad }
                },
                Transition {
                    to: "hideAll"
                    NumberAnimation { target: outputCore; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
                }
            ]
        }
    }

    PathView {
        id: pathView
        model: model
        delegate: delegate
        focus: focusPath
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
                currentFilm = counter+1 
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
            root.showOutput = false
            outputCore.state = "back"
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
    }
    
    function listUpdater(offset) {
        hideAll = true
        focusPath = false
        posterLoaded = false
        coverLoaded = false
        showWaitSpinner = true
        root.list = ''
        root.listUpdated = false
        outputCore.state = ''
        model.clear()
        root.findDb(inputText, category, pornstar, offset, false)
    }
}
