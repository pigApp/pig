import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool require_password
    property bool ok_password
    property bool fail_password
    property bool requireConfirmation
    property bool get
    property bool requireRestart
    property bool news
    property bool showWave 

    property string status
    property string information
    property string binaryNews: ""
    property string databaseNews: ""
    property string binaryVersion
    property string databaseVersion
    property string release
    property string bitRate

    property int totalFilms
    property int n
    property real strap: 1
    property int peers: 0
    property int required: 0
    property int downloaded: 0

    property variant categoryList
    property variant nCategoryList
    property variant pornstarList
    property variant nPornstarList
    property variant list

    signal passwordHandle(string plain, bool init, bool write)
    signal skip()
    signal getFiles()
    signal find(string inputText, string category, string pornstar, int offset, bool init)
    signal torrentHandle(string magnetUrl, string scenne, bool abort)
    signal quit()

    FontLoader { id: pigFont; source: "qrc:/images/font/pig.ttf" }
    
    Rectangle {
        id: screen
        color: "white"
        anchors.fill: parent

        Image {
            id: logo
            source: "qrc:/images/pig/logo.png"
            cache: false
            fillMode: Image.PreserveAspectCrop
            anchors.left: parent.left
            anchors.leftMargin: -270
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0
        }
        GaussianBlur {
            id: blur
            source: logo
            radius: 0
            samples: 32
            cached: true
            opacity: 0
            anchors.fill: logo
        }
        
        Loader {
            id: loader
            asynchronous: true
            focus: true
            visible: { status == Loader.Ready }
            anchors.fill: parent
            z: 2
        }

        Loader {
            id: loader_finder_output
            asynchronous: true
            visible: { status == Loader.Ready }
            anchors.fill: parent
            onZChanged: {
                if (z == 0) {
                    loader_finder_output.forceActiveFocus()
                    if (loader_finder_output.source == "qrc:/src/qml/Finder.qml") {
                        logo.visible = true
                        blur.visible = true
                    }
                }
            }
        }

        states: [
            State {
                name: "showSetPassword"
                PropertyChanges { target: loader; source: "SetPassword.qml" }
                PropertyChanges { target: logo; visible: false }
                PropertyChanges { target: blur; visible: false }
                PropertyChanges { target: loader_finder_output; z: 1 }
            },
            State {
                name: "showHelp"
                PropertyChanges { target: loader; source: "Help.qml" }
                PropertyChanges { target: loader_finder_output; z: 1 }
            },
            State {
                name: "cleanUp"
                PropertyChanges { target: loader; source: "" } 
                PropertyChanges { target: loader_finder_output; z: 0 }
            }
        ]
    }

    onRequire_passwordChanged: { loader.source = "AskPassword.qml" }

    Connections {
        target: cppSignals
        onShowUpdateSIGNAL: { loader.source = "Update.qml" }
        onStartSIGNAL: { loader.source = ""; loader_finder_output.source = "Finder.qml" }
        onShowErrorDbSIGNAL: { loader_finder_output.source = ""; loader.source = "ErrorDb.qml"; logo.visible = false; blur.visible = false }
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
