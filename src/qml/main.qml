import QtQuick 2.4
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool init
    property bool askPassword
    property bool showNetwork
    property bool network_err
    property string status
    property string information
    property string binary
    property string release
    property string database
    property string binaryNews: ""
    property string databaseNews: ""
    property string userInput: ""
    property string pornstar: ""
    property string category: ""
    property string full: ""
    property string quality: ""
    property string bitRate: ""
    property string videoPath: ""
    property int xa: screen.width
    property int xb: 0
    property int n_films
    property var data_films
    property int mb_required: 0
    property int mb_downloaded: 0
    property int n_mb: 0
    property int peers: 0
    property var categories
    property var pornstars
    property var n_categories
    property var n_pornstars
    property real screenOpacity: 0.4

    signal sig_qml_password_handler(bool require, string userInput, bool check, bool write)
    signal sig_qml_update_get()
    signal sig_qml_update_skip()
    signal sig_qml_find(string userInput, string pornstar, string category, string quality, string full)
    signal sig_qml_preview_handler(string host, string url, string path, string target, int id, bool success, bool abort)
    signal sig_qml_torrent_handler(string url, int scene, bool abort)
    signal sig_qml_quit()

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
        color: Qt.rgba(0, 0, 0, screenOpacity)
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
                PropertyChanges { target: loader_root_b; source: "finder/FinderHandler.qml"; restoreEntryValues: false }
                PropertyChanges { target: root; xb: 0; restoreEntryValues: false }
            },
            State {
                name: "show_viewer"
                PropertyChanges { target: root; xb: screen.width+50 }
                PropertyChanges { target: loader_root_b; source: ""; restoreEntryValues: false }
                PropertyChanges { target: loader_root_b; source: "viewer/ViewerHandler.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_torrent"
                PropertyChanges { target: loader_root; source: "torrent/TorrentHandler.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_torrent"
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
        target: cppSignals
        onSig_ret_password: {
            if (require) {
                askPassword = true
                screen.state = "show_password"
            }
        }
        onSig_show_update: {
            screen.state = "show_update"
        }
        onSig_show_news: {
            root.binaryNews = binaryNews
            root.databaseNews = databaseNews
            screen.state = "show_news"
        }
        onSig_show_finder: {
            screen.state = "show_finder"
        }
        onSig_ret_db: {
            root.n_films = nFilms
            root.data_films = dataFilms
            if (nFilms !== 0)
                if ((pornstar !== "") || (category !== ""))
                    loader_root_b.item.state = "hide_filter_finder"
                else
                    loader_root_b.item.state = "hide"
        }
        onSig_show_db_err: {
            screen.state = "show_db_err"
        }
    }
}
