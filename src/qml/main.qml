import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool welcome
    property bool require_password
    property bool ok_password
    property bool fail_password
    property bool requireConfirmation
    property bool get
    property bool requireRestart
    property bool news
    property bool onShowSelectors
    property bool showNetworkIcon
    property bool networkError

    property string status
    property string information
    property string binaryNews: ""
    property string databaseNews: ""
    property string binaryVersion
    property string databaseVersion
    property string input: ""
    property string pornstar: ""
    property string category: ""
    property string quality: ""
    property string full: ""
    property string release
    property string bitRate: ""

    property int xAnimation: screen.width
    property int totalFilms
    property int n
    property int peers: 0
    property int required: 0
    property int downloaded: 0

    property variant categoryList
    property variant nCategoryList
    property variant pornstarList
    property variant nPornstarList
    property variant list

    signal passwordHandleSIGNAL_QML(string plain, bool init, bool write)
    signal skipSIGNAL_QML()
    signal getFilesSIGNAL_QML()
    signal findSIGNAL_QML(string input, string pornstar, string category, string quality, string full, int offset, bool init)
    signal torrentHandleSIGNAL_QML(string magnet, int scene, int fit, bool abort)
    signal quitSIGNAL_QML()

    FontLoader { id: pigFont; source: "qrc:/resources/font/pig.ttf" }

    Rectangle {
        id: screen
        color: "white"
        anchors.fill: parent

        Loader {
            id: loader
            z: 2
            asynchronous: true
            focus: true
            visible: { status === Loader.Ready }
            anchors.fill: parent
        }
        Loader {
            id: loader_finder_output
            asynchronous: true
            visible: { status === Loader.Ready }
            anchors.fill: parent
        }

        states: [
            State {
                name: "showNews"
                PropertyChanges { target: loader; source: "News.qml" }
            },
            State {
                name: "hideNews"
                PropertyChanges { target: root; news: false }
                PropertyChanges { target: loader; source: "" }
            },
            State {
                name: "showSetPassword"
                PropertyChanges { target: loader; source: "SetPassword.qml" }
            },
            State {
                name: "hideSetPassword"
                PropertyChanges { target: loader; source: "" }
            },
            State {
                name: "showHelp"
                PropertyChanges { target: loader; source: "Help.qml" }
            },
            State {
                name: "hideHelp"
            }
        ]
        transitions: [
            Transition {
                to: "showHelp"
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: 0; duration: 1100; easing.type: Easing.OutQuart }            },
            Transition {
                to: "hideHelp"
                SequentialAnimation {
                    PropertyAction { target: root; property: "welcome"; value: false }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: loader; property: "source"; value: "" }
                    PropertyAction { target: loader_finder_output; property: "focus"; value: true }
                }
            }
        ]
    }

    onRequire_passwordChanged: { loader.source = "AskPassword.qml" }

    Connections {
        target: cppSignals
        onShowUpdateSIGNAL: { loader.source = "Update.qml" }
        onStartSIGNAL: { loader.source = ""; loader_finder_output.source = "Finder.qml" }
        onShowErrorDatabaseSIGNAL: { loader_finder_output.source = ""; loader.source = "ErrorDb.qml"; logo.visible = false; blur.visible = false }
    }
}
