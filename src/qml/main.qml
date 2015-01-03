import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool init
    property bool askPassword
    property bool showNetwork
    property bool errorNetwork
    property string status
    property string information
    property string binary
    property string release
    property string database
    property string binaryNews: ""
    property string databaseNews: ""
    property string inputUser: ""
    property string pornstar: ""
    property string category: ""
    property string full: ""
    property string quality: ""
    property string bitRate: ""
    property string videoFilePath: ""
    property int xA: screen.width
    property int xB: 0
    property int block_films
    property int total_films
    property int required: 0
    property int peers: 0
    property int downloaded: 0
    property int downloaded_kb: 0
    property int total_kb: 0
    property var categories
    property var pornstars
    property var totalCategories
    property var totalPornstars
    property var data_films
    property real screenOpacity: 0.4

    signal signal_qml_password_handler(string plain, bool require, bool check, bool write)
    signal signal_qml_accept_update()
    signal signal_qml_skip_update()
    signal signal_qml_find(string inputUser, string pornstar, string category, string quality, string full, int offset, bool init)
    signal signal_qml_preview_handler(string host, string url, string path, string target, int id, bool success, bool abort)
    signal signal_qml_torrent_handler(string magnet, int scene, bool abort)
    signal signal_qml_quit()

    FontLoader { id: globalFont; source: "qrc:/font-global" }
    FontLoader { id: fancyFont; source: "qrc:/font-fancy" }
    FontLoader { id: customFont; source: "qrc:/font-custom" }

    Image {
        id: background
        cache: false
        source: "qrc:/img-background"
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
                name: "show_password"
                PropertyChanges { target: root_loader_A; source: "Password.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_password"
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_B; focus: true; restoreEntryValues: false }
            },
            State {
                name: "show_update"
                PropertyChanges { target: root; askPassword: false; restoreEntryValues: false }
                PropertyChanges { target: background; visible: true; restoreEntryValues: false }
                PropertyChanges { target: backgroundBlur; visible: true; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: "Update.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_news"
                PropertyChanges { target: backgroundBlur; radius: 64; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: "News.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_help"
                PropertyChanges { target: root_loader_A; source: "global/Help.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_help"
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_B; focus: true; restoreEntryValues: false }

            },
            State {
                name: "show_finder"
                PropertyChanges { target: root; pornstar: ""; restoreEntryValues: false }
                PropertyChanges { target: root; category: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_B; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_B; source: "finder/FinderHandler.qml"; restoreEntryValues: false }
                PropertyChanges { target: root; xB: 0; restoreEntryValues: false }
            },
            State {
                name: "show_viewer"
                PropertyChanges { target: root; xB: screen.width+50 }
                PropertyChanges { target: root_loader_B; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_B; source: "viewer/ViewerHandler.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_torrent"
                PropertyChanges { target: root_loader_A; source: "torrent/TorrentHandler.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_torrent"
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
            },
            State {
                name: "show_errorDb"
                PropertyChanges { target: root_loader_B; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: "global/ErrorDb.qml"; restoreEntryValues: false }
            }
        ]
    }

    Connections {
        target: cppSignals
        onSignal_ret_password: {
            if (require) {
                askPassword = true
                screen.state = "show_password"
            }
        }
        onSignal_show_update: {
            screen.state = "show_update"
        }
        onSignal_show_news: {
            root.binaryNews = binaryNews
            root.databaseNews = databaseNews
            screen.state = "show_news"
        }
        onSignal_show_finder: {
            screen.state = "show_finder"
        }
        onSignal_ret_db: {
            root.block_films = blockFilms
            root.data_films = dataFilms
            if (blockFilms !== 0 && !updateData)
                if (pornstar !== "" || category !== "")
                    root_loader_B.item.state = "hide_filter_finder"
                else
                   root_loader_B.item.state = "hide"
            else if (updateData)
                root_loader_B.item.append_data()
        }
        onSignal_show_errorDatabase: {
            screen.state = "show_errorDb"
        }
    }
}
