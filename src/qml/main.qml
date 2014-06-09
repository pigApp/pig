import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool requirePass
    property bool okPass
    property bool failPass
    property bool showCloseButton: true
    property bool showSpinner 
    property bool showDecisionButton
    property bool restart
    property bool authorize

    property string status
    property string statusInformation
    property string binaryVersion
    property string binaryRelease
    property string dataBaseVersion

    property variant categoryList
    property variant nCategoryList
    property variant pornstarList
    property variant nPornstarList
    property variant list

    property int totalFilms
    property int n
    property real strap:1

    signal passwordHandle(string plain, bool init, bool write)
    signal updateAccept()
    signal updateCancel()
    signal updateRestart()
    signal findDb(string inputText, string category, string pornstar, int offset, bool init)
    signal getTorrent(string host, string url, string scenneID)
    signal fixDb()
    signal quit()

    FontLoader { id: pigFont; source: "qrc:/images/font/pig.ttf" }
    FontLoader { id: pigLightFont; source: "qrc:/images/font/pigLight.ttf" }
    
    Rectangle {
        id: screen
        color: "white"
        anchors.fill: parent

        Image {
            id: pigLogo
            source: "qrc:/images/pig.png"
            cache: false
            fillMode: Image.PreserveAspectCrop
            anchors.left: parent.left
            anchors.leftMargin: -270
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 70
            opacity: 0
        }
        GaussianBlur {
            id: blur
            source: pigLogo
            radius: 0
            samples: 32
            cached: true
            opacity: 0
            anchors.fill: pigLogo
        }
        
        Loader {
            id: loaderAskPassword
            source: "qrc:/src/qml/AskPassword.qml"
            active: false
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
        }
        Loader {
            id: loaderUpdate
            source: "qrc:/src/qml/Update.qml"
            active: false
            visible: status == Loader.Ready
            anchors.fill: parent
        }
        Loader {
            id: loaderFinder
            source: "qrc:/src/qml/Finder.qml"
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
            source: "qrc:/src/qml/Output.qml"
            active: false
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
            onStatusChanged: { if (status == Loader.Ready) loaderFinder.source = "" }
            onActiveChanged: { if (loaderOutput.active) showSpinner = true }
        }

        Loader {
            id: loaderWaitMsg
            source: "qrc:/src/qml/WaitMsg.qml"
            active: false
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
        }

        Loader {
            id: loaderErrorDbMsg
            source: "qrc:/src/qml/ErrorDbMsg.qml"
            active: false
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
            onActiveChanged: {
                loaderAskPassword.source = ""
                loaderUpdate.source = ""
                loaderFinder.source = ""
                loaderOutput.source = ""
                loaderWaitMsg.source = ""
            }
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
        onShowErrorDbMsgSIGNAL: {
            loaderErrorDbMsg.active = true
        }    
    }

    Component.onCompleted: {
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

// TODO: TimeOut's.
// TODO: Enviar updateErrorDbSIGNAL desde update.
// TODO: quit en waitMsg AskPassword y en c++.
