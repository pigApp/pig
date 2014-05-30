import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool requirePass
    property bool okPass
    property bool failPass
    property bool showCloseButton: true
    property string status
    property string statusInformation
    property real strap:1

    property bool showSpinner
    property bool showDecisionButton
    property bool restart
    property bool authorize
    property bool showFixDbButton

    property bool showDbError
    property bool showWaitSpinner
    property bool transition
    property string binaryVersion
    property string dataBaseVersion
    property string release
    property variant categoryList
    property variant nCategoryList
    property variant pornstarList
    property variant nPornstarList

    property bool playerClosed
    property variant list
    property int totalFilms
    property int n

    signal passManager(string plain, bool init, bool write)
    signal updateAccept()
    signal updateCancel()
    signal updateRestart()
    signal findDb(string inputText, string category, string pornstar, int offset, bool init)
    signal openPlayer(string videoID)
    signal fixDb()
    signal quit()

    FontLoader { id: pigFont; source: "qrc:/images/font/pig.ttf" }
    FontLoader { id: pigLightFont; source: "qrc:/images/font/pigLight.ttf" }
    
    Rectangle {
        id: screen
        color: "white"
        anchors.fill: parent

        Image {
            id: girl
            source: "qrc:/images/girl.png"
            cache: false
            fillMode: Image.PreserveAspectCrop
            opacity: 0
        }
        GaussianBlur {
            id: blur
            source: girl
            radius: 0
            samples: 32
            cached: true
            opacity: 0
            anchors.fill: girl
        }
        
        Loader {
            id: loaderAskPassword
            source: "qrc:/src/qml/core/AskPassword.qml"
            active: false
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
        }
        Loader {
            id: loaderUpdate
            source: "qrc:/src/qml/core/Update.qml"
            active: false
            visible: status == Loader.Ready
            anchors.fill: parent
        }
        Loader {
            id: loaderFinder
            source: "qrc:/src/qml/core/Finder.qml"
            active: false
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
            onStatusChanged: { 
                if (status == Loader.Ready) 
                    if (loaderOutput.active)
                        loaderOutput.source = "" 
                }
        }
        Loader {
            id: loaderOutput
            source: "qrc:/src/qml/core/Output.qml"
            active: false
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
            onStatusChanged: { if (status == Loader.Ready) loaderFinder.source = "" }
        }

        Image {// Qt.quit
            id: closeButton 
            width: 14
            height: 12
            source: "qrc:/images/close.png"
            visible: showCloseButton
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: -1
            z:3
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: { quit() }
                onEntered: { closeButton.source = "qrc:/images/closeHover.png" }
                onHoveredChanged: { closeButton.source = "qrc:/images/close.png" }
            }
        }
    }

    onStatusChanged: {
        if (status === 'searching updates' || 'ERROR IN DATABASE') {
            if (root.width >= 1920)
                strap = 1
            else if (root.width > 1399 && root.width < 1920)
                strap = 1.1
            else if (root.width > 1023 && root.width < 1400)
                strap = 1.2
            else
                strap = 1
        }
    }

    onRequirePassChanged: {
        loaderAskPassword.active = true
        loaderAskPassword.focus = true
        loaderUpdate.asynchronous = true
    }
    
    Connections {
        target: cppSignals

        onShowUpdateSIGNAL: {
            loaderAskPassword.source = ""
            loaderAskPassword.active = false
            loaderAskPassword.focus = false
            loaderUpdate.active = true
        }
        onShowFinderSIGNAL: {
            loaderAskPassword.source = ""
            loaderAskPassword.active = false
            loaderAskPassword.focus = false
            
            loaderUpdate.source = ""
            loaderUpdate.active = false

            loaderFinder.active = true
            loaderFinder.focus = true
        }
    }
}

// TODO: Spinner como component.
