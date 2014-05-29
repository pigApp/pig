import QtQuick 2.1
import QtGraphicalEffects 1.0
import signals 1.0
//import "core" 1.0 as Core

Item {
    id: root

    SIGNALS {
        onSignalShowFinder: {
            console.log("/////TRIGGEREDSHOW")

            if (initiation) {
                if (!requirePass)
                    loaderAskPassword.source = ""
                loaderUpdate.source = ""
                loaderUpdate.active = false

                loaderFinder.active = true
                loaderFinder.focus = true

                initiation = false
            }
        }
    }

    property bool initiation: true
    property bool requirePass
    property bool showAskPass
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

    property bool showFinder
    property bool showFinderFast
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
    property bool noResult

    property bool showOutput
    property bool listUpdated
    property bool playerClosed
    property variant list
    property variant basicData
    property int totalFilms
    property int n

    property alias girlOpacity: girl.opacity
    property alias blurOpacity: blur.opacity
    property alias blurRadius: blur.radius

    property alias sourceFinder: loaderFinder.source
    property alias activeFinder: loaderFinder.active
    property alias activeOutput: loaderOutput.active

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
            visible: status == Loader.Ready
            anchors.fill: parent
        }
        Loader {
            id: loaderFinder
            source: "qrc:/src/qml/core/Finder.qml"
            active: false//showFinder
            //focus: showFinder
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
        }
        Loader {
            id: loaderOutput
            source: "qrc:/src/qml/core/Output.qml"
            active: false//showOutput
            //focus: showOutput
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
            onStatusChanged: { if (status == Loader.Ready) loaderFinder.source = ""; loaderFinder.active = false }
        }

        /*
        Core.Finder {
            id: finderCore
            opacity: 0
            enabled: false
            anchors.fill: parent
            states: [
                State {
                    name: "show"
                    when: showFinder
                }
            ]
            transitions: [
                Transition {
                    to: "show"
                    PropertyAction { target: finderCore; property: "opacity"; value: 1.0 }
                    PropertyAction { target: finderCore; property: "enabled"; value: true }
                }
            ]
        }
        */
        /*
        Core.Output {
            id: outputCore
            opacity: 0
            enabled: false
            anchors.fill: parent
            states: [
                State {
                    name: "show";
                    when: showOutput
                    //PropertyChanges { target: finderCore; enabled: false }
                    PropertyChanges { target: outputCore; enabled: true }
                    StateChangeScript {
                        name:"listCreator"
                        script: outputCore.listCreator(n, list);
                    }
                },
                State {
                    name: "updateList";
                    when: listUpdated
                    StateChangeScript {
                        name:"listCreator"
                        script: outputCore.listCreator();
                    }
                },
                State {
                    name: "back"
                    PropertyChanges { target: outputCore; enabled: false }
                    //PropertyChanges { target: finderCore; opacity: 1.0 }
                    //PropertyChanges { target: finderCore; enabled: true }
                }
            ]
            transitions: [
                Transition {
                    to: "show"
                    ScriptAction { scriptName:"listCreator" }
                    PropertyAction { target: root; property: "status"; value: "" }
                    PropertyAction { target: root; property: "statusInformation"; value: "" }
                },
                Transition {
                    to: "updateList"
                    ScriptAction { scriptName:"listCreator" }
                },
                Transition {
                    to: "back"
                    SequentialAnimation {
                        NumberAnimation { target: outputCore; property: "opacity"; to: 0; duration: 700; easing.type: Easing.InOutQuad }
                        PropertyAction { target: root; property: "showFinderFast"; value: true }
                   }
                }
            ]
        }
        */

        Image {
            id: closeButton // Cambiar esto por un shortcut Esc+shift
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
    onShowAskPassChanged: {
        if (!showAskPass) {
            loaderAskPassword.source = ""
            loaderAskPassword.active = false
            loaderAskPassword.focus = false
        }
    }

    /*
    onShowFinderChanged: {
        if (initiation) {
            if (!requirePass)
                loaderAskPassword.source = ""
            loaderUpdate.source = ""
            loaderUpdate.active = false
            initiation = false
        }
    }
    */
}
