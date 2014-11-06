import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool welcome
    property bool forceShowFinder
    property bool showNetwork
    property bool errorNetwork
    property bool requireConfirmation
    property bool get
    property bool requireRestart
    property bool showUserInputLabel
    property bool hideFinder_showOutput
    property bool hideFinder_hideFilters_showOutput

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

    signal password_handle_qml_signal(string plain, bool write)
    signal skip_qml_signal()
    signal get_files_qml_signal()
    signal find_qml_signal(string input, string pornstar, string category, string quality, string full, int offset, bool init)
    signal preview_handle_qml_signal(string host, string url, string path, string file, int id, bool success, bool fail)
    signal torrent_handle_qml_signal(string magnet, int scene, bool abort)
    signal quit_qml_signal()

    FontLoader { id: pigFont; source: "/resources/font/pig.ttf" }

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
                name: "showWelcome"
                PropertyChanges { target: loader; source: "Welcome.qml" }
            },
            State {
                name: "hideWelcome"
            },
            State {
                name: "showNews"
                PropertyChanges { target: loader; source: "News.qml" }
            },
            State {
                name: "hideNews"
                PropertyChanges { target: loader; source: "" }
                PropertyChanges { target: root; forceShowFinder: true }
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
                to: "hideWelcome"
                SequentialAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: loader; property: "source"; value: "" }
                    PropertyAction { target: root; property: "forceShowFinder"; value: "true" }
                    PropertyAction { target: root; property: "showUserInputLabel"; value: "true" }
                }
            },
            Transition {
                to: "showHelp"
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
            },
            Transition {
                to: "hideHelp"
                SequentialAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: loader; property: "source"; value: "" }
                    PropertyAction { target: loader_finder_output; property: "focus"; value: true }
                }
            }
        ]
    }

    onForceShowFinderChanged: {
        if (forceShowFinder) {
            loader_finder_output.source = "Finder.qml"
            forceShowFinder = false
       }
    }

    Connections {
        target: cppSignals
        onShow_welcome_signal: { screen.state = "showWelcome"; welcome = true}
        onRequire_password_signal: { loader.source = "AskPassword.qml" }
        onShow_update_signal: { loader.source = "Update.qml" }
        onShow_news_signal: {
            root.binaryNews = binaryNews
            root.databaseNews = databaseNews
            screen.state = "showNews"
        }
        onShow_finder_signal: { loader.source = ""; loader_finder_output.source = "Finder.qml" }
        onShow_output_signal: {
            root.n = n
            root.list = list
            if (pornstar !== "" || category !== "")
                hideFinder_hideFilters_showOutput = true
            else
                hideFinder_showOutput = true
        }
        onShow_errorDatabase_signal: { loader_finder_output.source = ""; loader.source = "ErrorDb.qml" }
    }
}
