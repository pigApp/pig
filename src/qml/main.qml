import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool welcome
    property bool showNetwork
    property bool errorNetwork
    property bool requireConfirmation
    property bool get
    property bool requireRestart
    property bool showUserInputLabel
    property bool hideHelp
    property bool hideFinder_showOutput
    property bool hideFilters_hideFinder_showOutput
    property bool stopPreview

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
    property int screenWidth
    property int peers: 0
    property int required: 0
    property int downloaded: 0

    property variant categoryList
    property variant nCategoryList
    property variant pornstarList
    property variant nPornstarList
    property variant list

    signal password_handler_qml_signal(string plain, bool write)
    signal skip_qml_signal()
    signal get_files_qml_signal()
    signal find_qml_signal(string input, string pornstar, string category, string quality, string full, int offset, bool init)
    signal preview_handler_qml_signal(string host, string url, string path, string file, int id, bool success, bool fail, bool abort)
    signal torrent_handler_qml_signal(string magnet, int scene, bool stop)
    signal quit_qml_signal()

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
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root; property: "showUserInputLabel"; value: true }
                    PropertyAction { target: root; property: "welcome"; value: false }
                }
            },
            Transition {
                to: "showHelp"
                NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
            },
            Transition {
                to: "hideHelp"
                SequentialAnimation {
                    PropertyAction { target: root; property: "hideHelp"; value: true }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xAnimation"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root_loader_B; property: "focus"; value: true }
                    PropertyAction { target: root; property: "hideHelp"; value: false }
                }
            }
        ]
    }

    Connections {
        target: cppSignals
        onShow_welcome_signal: {
            screen.state = "showWelcome"
            welcome = true
        }
        onRequire_password_signal: { root_loader_A.source = "AskPassword.qml" }
        onShow_update_signal: { root_loader_A.source = "Update.qml" }
        onShow_news_signal: {
            root.binaryNews = binaryNews
            root.databaseNews = databaseNews
            screen.state = "showNews"
        }
        onShow_finder_signal: {
            root_loader_A.source = ""
            root_loader_B.source = "Finder.qml"
        }
        onShow_output_signal: {
            root.n = n
            root.list = list
            if (pornstar !== "" || category !== "")
                hideFilters_hideFinder_showOutput = true
            else
                hideFinder_showOutput = true
        }
        onShow_errorDatabase_signal: {
            root_loader_B.source = ""
            root_loader_A.source = "ErrorDb.qml"
        }
    }
}
