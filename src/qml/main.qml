import QtQuick 2.4
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool onInit
    property bool askPassword
    property bool showNetwork
    property bool onHelpFinder
    property bool preview_quit
    property bool network_err
    property string tmp
    property string status
    property string information
    property string binary
    property string release
    property string database
    property string binaryNews: ""
    property string databaseNews: ""
    property string quality: ""
    property string full: ""
    property string category: ""
    property string pornstar: ""
    property string userInput: ""
    property string bitRate: ""
    property string movie_file: ""
    property int xa: screen.width
    property int xb: 0
    property int n_movies
    property int mb_required: 0
    property int mb_downloaded: 0
    property int n_mb: 0
    property int peers: 0
    property var data_movies
    property var categories
    property var pornstars
    property var n_categories
    property var n_pornstars
    property var cache_cover_front: []
    property var cache_cover_back: []
    property var cache_preview: []
    property real screenR: 0
    property real screenG: 0
    property real screenB: 0
    property real screenA: 0.4

    signal sig_qml_update_get()
    signal sig_qml_update_skip()

    FontLoader { id: fontGlobal; source: "qrc:/font-global" }

    Image {
        id: background
        source: "qrc:/img-background"
        cache: false
        visible: false
        anchors.fill: parent
    }
    FastBlur {
        id: backgroundBlur
        source: background
        radius: 32
        visible: false
        anchors.fill: background
    }

    Rectangle {
        id: screen
        color: Qt.rgba(screenR, screenG, screenB, screenA)
        anchors.fill: parent

        Loader {
            id: loader_root
            z: 2
            focus: true
            asynchronous: true
            visible: { status === Loader.Ready }
            anchors.fill: parent
        }
        Loader {
            id: loader_root_b
            asynchronous: true
            visible: { status === Loader.Ready }
            anchors.fill: parent
        }

        states: [
            State {
                name: "show_password"
                PropertyChanges { target: loader_root; source: "Password.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_password"
                PropertyChanges { target: loader_root; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; focus: true; restoreEntryValues: false }
            },
            State {
                name: "show_update"
                PropertyChanges { target: root; askPassword: false; restoreEntryValues: false }
                PropertyChanges { target: background; visible: true; restoreEntryValues: false }
                PropertyChanges { target: backgroundBlur; visible: true; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: "Update.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_news"
                PropertyChanges { target: loader_root; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: "News.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_help"
                PropertyChanges { target: loader_root; source: "global/Help.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_help"
                PropertyChanges { target: loader_root; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; focus: true; restoreEntryValues: false }

            },
            State {
                name: "show_finder"
                PropertyChanges { target: root; pornstar: ""; restoreEntryValues: false }
                PropertyChanges { target: root; category: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; source: "finder/Finder.qml"; restoreEntryValues: false }
                PropertyChanges { target: root; xb: 0; restoreEntryValues: false }
            },
            State {
                name: "show_viewer"
                PropertyChanges { target: root; xb: screen.width+50 }
                PropertyChanges { target: loader_root_b; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; source: "viewer/Viewer.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_movie"
                PropertyChanges { target: loader_root; source: "movie/Movie.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_movie"
                PropertyChanges { target: loader_root; source: ""; restoreEntryValues: false }
            },
            State {
                name: "show_db_err"
                PropertyChanges { target: loader_root_b; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: "global/Db_err.qml"; restoreEntryValues: false }
            }
        ]
    }

    Connections {
        target: cpp
        onSig_ret_password: {
            if (require) {
                askPassword = true
                screen.state = "show_password"
            }
        }
        onSig_show_update: { screen.state = "show_update" }
        onSig_show_news: {
            root.binaryNews = binaryNews
            root.databaseNews = databaseNews
            screen.state = "show_news"
        }
        onSig_show_finder: { screen.state = "show_finder" }
        onSig_ret_db: {
            root.n_movies = nMovies
            root.data_movies = dataMovies
            if (nMovies !== 0)
                if ((category !== "") || (pornstar !== ""))
                    loader_root_b.item.state = "hide_filter_finder"
                else
                    loader_root_b.item.state = "hide"
        }
        onSig_show_db_err: { screen.state = "show_db_err" }
    }
}
