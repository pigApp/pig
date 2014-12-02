import QtQuick 2.3
import QtGraphicalEffects 1.0

Item {
    id: root

    property bool init
    property bool askPassword
    property bool showNetwork
    property bool errorNetwork
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
    property int xA: screen.width
    property int xB: 0
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
    signal signal_qml_torrent_handler(string magnet, int scene, bool abort)
    signal signal_qml_quit()

    FontLoader { id: pigFont; source: "/resources/fonts/pig.ttf" }
    FontLoader { id: finderFont; source: "/resources/fonts/finder.ttf" }

    Image {
        id: background
        source: "/resources/images/general/background.jpg"
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
        color: Qt.rgba(0, 0, 0, 0.6)
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
                PropertyChanges { target: root_loader_A; source: "Update.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_news"
                PropertyChanges { target: backgroundBlur; radius: 64; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: "News.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_news"
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_B; source: "Finder.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_finder"
                PropertyChanges { target: root; pornstar: ""; restoreEntryValues: false }
                PropertyChanges { target: root; category: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_B; source: "Finder.qml"; restoreEntryValues: false }
                PropertyChanges { target: root; xB: 0; restoreEntryValues: false }
            },
            State {
                name: "show_help"
                PropertyChanges { target: root_loader_A; source: "Help.qml"; restoreEntryValues: false }
            },
            State {
                name: "hide_help"
            },
            State {
                name: "show_viewer"
                PropertyChanges { target: root; xB: screen.width+50 }
                PropertyChanges { target: root_loader_B; source: "Viewer.qml"; restoreEntryValues: false }
            },
            State {
                name: "show_errorDb"
                PropertyChanges { target: root_loader_B; source: ""; restoreEntryValues: false }
                PropertyChanges { target: root_loader_A; source: "ErrorDb.qml"; restoreEntryValues: false }
            }
        ]
        transitions: [
            Transition {
                to: "show_help"
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
                    NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 96; duration: 2000; easing.type: Easing.OutQuart }
                }
            },
            Transition {
                to: "hide_help"
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xA"; to: screen.width; duration: 1100; easing.type: Easing.OutQuart }
                        NumberAnimation { target: root; easing.amplitude: 1.7; properties: "xB"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                        NumberAnimation { target: backgroundBlur; easing.amplitude: 1.7; properties: "radius"; to: 0; duration: 2100; easing.type: Easing.OutQuart }
                    }
                    PropertyAction { target: root_loader_A; property: "source"; value: "" }
                    PropertyAction { target: root_loader_B; property: "focus"; value: true }
                }
            }
        ]
    }

    Connections {
        target: cppSignals
        onSignal_require_password: {
            askPassword = true
            screen.state = "show_password"
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
            root.nFilms = nFilms
            root.dataFilms = dataFilms
            if (nFilms !== 0 && !updateData)
                if (pornstar !== "" || category !== "")
                    root_loader_B.item.state = "hide_filter_finder"
                else
                   root_loader_B.item.state = "hide"
            else if (updateData)
                root_loader_B.item.appendData()
        }
        onSignal_show_errorDatabase: {
            screen.state = "show_errorDb"
        }
    }
}
