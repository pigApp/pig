import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool welcome
    property bool showNetwork
    property bool errorNetwork
    property bool showUserInputLabel
    property bool hideHelp
    property bool hideFinder_showOutput
    property bool hideFilters_hideFinder_showOutput
    property bool stopPreview
    property string status
    property string information
    property string binary
    property string release
    property string database
    property string binaryNews: ""
    property string databaseNews: ""
    property string input: ""
    property string pornstar: ""
    property string category: ""
    property string quality: ""
    property string full: ""
    property string bitRate: ""
    property int xtransition: screen.width
    property int nFilms
    property int totalFilms
    property int screenWidth
    property int peers: 0
    property int required: 0
    property int downloaded: 0
    property variant categories
    property variant nCategories
    property variant pornstars
    property variant nPornstars
    property variant dataFilms

    signal signal_qml_password_handler(string plain, bool require, bool check, bool write)
    signal signal_qml_find(string input, string pornstar, string category, string quality, string full, int offset, bool init)
    signal signal_qml_preview_handler(string host, string url, string path, string target, int id, bool success, bool fail, bool abort)
    signal signal_qml_torrent_handler(string magnet, int scene, bool stop)
    signal signal_qml_quit()

    FontLoader { id: pigFont; source: "/resources/fonts/pig.ttf" }
    FontLoader { id: finderFont; source: "/resources/fonts/finder.ttf" }

    Rectangle {
        id: screen
        color: "white"
        anchors.fill: parent

        Loader {
            id: root_loader_A
            z: 2
            asynchronous: true
            focus: true
            visible: { status === Loader.Ready }
            anchors.fill: parent
        }
        Loader {
            id: root_loader_B
            asynchronous: true
            visible: { status === Loader.Ready }
            anchors.fill: parent
        }

        states: [
            State {
                name: "showWelcome"
                PropertyChanges { target: root_loader_A; source: "Welcome.qml" }
            },
            State {
                name: "hideWelcome"
            },
            State {
                name: "showNews"
                PropertyChanges { target: root_loader_A; source: "News.qml" }
            },
            State {
                name: "hideNews"
                PropertyChanges { target: root_loader_A; source: "" }
                PropertyChanges { target: root_loader_B; source: "Finder.qml" }
            },
            State {
                name: "showSetPassword"
                PropertyChanges { target: root_loader_A; source: "SetPassword.qml" }
            },
            State {
                name: "hideSetPassword"
                PropertyChanges { target: root_loader_A; source: "" }
            },
            State {
                name: "showHelp"
                PropertyChanges { target: root_loader_A; source: "Help.qml" }
            },
            State {
                name: "hideHelp"
            }
        ]
        transitions: [
            Transition {
                to: "hideWelcome"
                SequentialAnimation {
                    PropertyAction { target: root_loader_B; property: "source"; value: "Finder.qml" }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xtransition"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root; property: "showUserInputLabel"; value: true }
                    PropertyAction { target: root; property: "welcome"; value: false }
                }
            },
            Transition {
                to: "showHelp"
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xtransition"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
            },
            Transition {
                to: "hideHelp"
                SequentialAnimation {
                    PropertyAction { target: root; property: "hideHelp"; value: true }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xtransition"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root_loader_B; property: "focus"; value: true }
                    PropertyAction { target: root; property: "hideHelp"; value: false }
                }
            }
        ]
    }

    Connections {
        target: cppSignals
        onSignal_show_welcome: {
            screen.state = "showWelcome"
            welcome = true
        }
        onSignal_require_password: { root_loader_A.source = "AskPassword.qml" }
        onSignal_show_update: { root_loader_A.source = "Update.qml" }
        onSignal_show_news: {
            root.binaryNews = binaryNews
            root.databaseNews = databaseNews
            screen.state = "showNews"
        }
        onSignal_show_finder: {
            root_loader_A.source = ""
            root_loader_B.source = "Finder.qml"
        }
        onSignal_show_output: {
            root.nFilms = nFilms
            root.dataFilms = dataFilms
            if (pornstar !== "" || category !== "")
                hideFilters_hideFinder_showOutput = true
            else
                hideFinder_showOutput = true
        }
        onSignal_show_errorDatabase: {
            root_loader_B.source = ""
            root_loader_A.source = "ErrorDb.qml"
        }
    }
}
