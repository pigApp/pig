import QtQuick 2.4
import QtGraphicalEffects 1.0

Item {
    id: root
    enabled: active
    visible: active

    property bool active: true
    property bool network
    property bool network_err
    property bool stop_preview_quit
    property string status
    property string information
    property string binary
    property string release
    property string database
    property string binaryNews: ""
    property string databaseNews: ""
    property string userInput: ""
    property string category: ""
    property string pornstar: ""
    property string quality: ""
    property string full: ""
    property string bitRate: ""
    property string tmp
    property int blurRadius: 32
    property int xa: screen.width
    property int xb: 0
    property int n_movies
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
        anchors.fill: parent
    }
    FastBlur {
        id: backgroundBlur
        source: background
        radius: blurRadius
        anchors.fill: background
    }

    Rectangle {
        id: screen
        color: Qt.rgba(screenR, screenG, screenB, screenA)
        anchors.fill: parent

        Loader {
            id: loader_root
            z: 2
            asynchronous: true
            focus: true
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
                name: "ask_password"
                PropertyChanges { target: loader_root; source: "Password.qml"
                    ; restoreEntryValues: false }
            },
            State {
                name: "show_update"
                PropertyChanges { target: loader_root; source: "Update.qml"
                    ; restoreEntryValues: false }
            },
            State {
                name: "show_news"
                PropertyChanges { target: loader_root; source: "News.qml"
                    ; restoreEntryValues: false }
            },
            State {
                name: "show_setting"
                PropertyChanges { target: loader_root_b; visible: false
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; enabled: false
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; screenA: 0.4
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; blurRadius: 32
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: "Setting.qml"
                    ; restoreEntryValues: false }
            },
            State {
                name: "hide_setting"
                PropertyChanges { target: loader_root; source: ""
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; visible: true
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; enabled: true
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; focus: true
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; screenA: 0
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; blurRadius: 0
                    ; restoreEntryValues: false }
            },
            State {
                name: "show_finder"
                PropertyChanges { target: root; category: ""
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; pornstar: ""
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: ""
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; source: "Finder.qml"
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; xb: 0
                    ; restoreEntryValues: false }
            },
            State {
                name: "show_viewer"
                PropertyChanges { target: root; xb: screen.width+50 }
                PropertyChanges { target: loader_root_b; source: "Viewer.qml"
                    ; restoreEntryValues: false }
            },
            State {
                name: "show_torrent_status"
                PropertyChanges { target: root; status: "TORRENT"
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: "TorrentStatus.qml"
                    ; restoreEntryValues: false }
            },
            State {
                name: "hide_torrent_status"
                PropertyChanges { target: loader_root; source: ""
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; status: ""
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; bitRate: ""
                    ; restoreEntryValues: false }
            },
            State {
                name: "show_db_error"
                PropertyChanges { target: loader_root_b; source: ""
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; screenR: 1
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; screenG: 0
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; screenB: 0
                    ; restoreEntryValues: false }
                PropertyChanges { target: root; screenA: 0.4
                    ; restoreEntryValues: false }
                PropertyChanges { target: loader_root; source: "Db_error.qml"
                    ; restoreEntryValues: false }
            }
        ]
    }

    Connections {
        target: cpp
        onSig_ret_password: { if (require) screen.state = "ask_password" }
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
        onSig_show_db_err: { screen.state = "show_db_error" }
    }
}
// Tabs hechos.
