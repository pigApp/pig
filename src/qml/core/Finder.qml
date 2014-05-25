import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: finder

    property bool showFilters
    property string enabledFilters
    property string category: ''
    property string pornstar: ''
    property int widthFilters: screen.width/4

    Loader {
        id: loaderUpdate
        source: "Update.qml"
        active: !showFinder
        anchors.fill: parent
        onActiveChanged: { if(!loaderUpdate.active) input.focus = true; loaderUpdate.destroy() }
    }

    Row {
        id: inputFindRow
        height: 10
        spacing: 7
        visible: buttonsFindColumn.visible && buttonsFindColumn.opacity == 1
        anchors.centerIn: parent
        TextInput {
            id: input
            color: Qt.rgba(0.2, 0.2, 0.2, 0.4)
            font.family: pigFont.name
            font.capitalization: Font.AllUppercase
            font.pixelSize: 25
            focus: true
            enabled: showFinder && !showDbError
            maximumLength: 29
            onAccepted: {
                if (!waitForResultTimer.running) {
                    if (!showFilters && !loaderSetPassword.active) {
                        if (noResultLabel.visible) { noResultLabel.visible = false }
                        root.findDb(input.text, category, pornstar, 0, true)
                        waitForResultTimer.start()
                    }
                }
            }
            Timer {
                id: waitForResultTimer
                running: false
                repeat: false
                interval: 1000// ver el tiempo
                onTriggered: {
                    if (!noResult) {
                        finder.state = "hideFinder"
                    }else {
                        root.noResult = false
                        noResultLabel.visible = true
                    }
                }
            }
            onCursorPositionChanged: { if (noResultLabel.visible) noResultLabel.visible = false }
            Keys.onPressed: {
                if (event.key === Qt.Key_P && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier) && showFinder && !root.requirePass) {
                    if (finder.state == 'showFinder' || finder.state == 'hideFilter') {
                        buttonsFindColumn.visible = false
                        inputFindRow.visible = false
                        input.focus = false
                        loaderSetPassword.active = true
                    }
                    event.accepted = true;
                }
            }
        }
        Text {
            id: noResultLabel
            text: "\\ NO RESULT \\"
            color: Qt.rgba(0.2, 0.2, 0.2, 0.2)
            font.family: pigFont.name
            font.pixelSize: 24/strap
            visible: false
        }
    }

    Column {
        id: buttonsFindColumn
        spacing: 5
        opacity: 0
        visible: showFinder && !showDbError
        enabled: showFinder && !showDbError
        anchors.left: parent.left
        anchors.leftMargin: 40
        anchors.verticalCenter: parent.verticalCenter
        z: 2
        Button {
            id: categoryFind
            width: 470
            height: 65
            label: "CATEGORY"
            onClicked: { finder.state = "showFilter"; filtersManager(categoryFind,null) }
        }
        Button {
            id: pornstarFind
            width: 470
            height: 65
            label: "PORNSTAR"
            onClicked: { finder.state = "showFilter"; filtersManager(pornstarFind,null) }
        }
    }
    Rectangle {
        id: filtersLayer
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.right
    }
    Loader {
        id: loaderFilters
        source: "Filters.qml"
        active: showFinder
        anchors.fill: parent
    }
    function filtersManager(filterName, label) {
        if (showFilters) {
            showFilters = false
            if (label !== null) {
               if (filterName === 'categoryFind')
                   category = label.toUpperCase()
               else
                   pornstar = label.toUpperCase()
               root.findDb(input.text, category, pornstar, 0, true)
               finder.state = "hideFilter_hideFinder"
            }else {
                finder.state = "hideFilter"
            }
        }else {
            enabledFilters = filterName.label
            bypassFilters.start()
        }
    }
    Timer {
        id: bypassFilters
        running: false
        repeat: false
        interval: 1150
        onTriggered: showFilters = true
    } 

    Row {
        id: waitRow
        spacing: 15
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -27
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: waitMsg
            text: "PLEASE WAIT"
            color: Qt.rgba(0.2, 0.2, 0.2, 0.4)
            font.family: pigFont.name
            font.pixelSize: 25
            opacity: { if(root.showWaitSpinner) 1; else 0 }
        }
        Image {
            id: waitSpinner
            width: 29.3
            height: 29.3
            source: "qrc:/images/spinner.png";
            visible: root.showWaitSpinner
            property bool on: root.showWaitSpinner
            NumberAnimation on rotation { running: waitSpinner.on; from: 0; to: 360; loops: Animation.Infinite; duration: 1200 }
        }
    }

    Loader {
        id: loaderSetPassword
        source: "SetPassword.qml"
        active: false
        //focus: true  Ver aca el tema del foco.
        anchors.fill: parent
    }

    Keys.onEscapePressed: {
        if (showFilters) {
            filtersManager(null,null)
            input.focus = true
        } else {
            loaderSetPassword.active = false
            buttonsFindColumn.visible = true
            inputFindRow.visible = true
            input.focus = true
        }
    }

    Text {
        id: dbErrorLabel
        text: status
        color: "white"
        font.pixelSize: 18/strap
        textFormat: Text.RichText
        visible: showDbError
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        id: dbErrorInformationLabel
        text: statusInformation
        color: Qt.rgba(0.5, 0.5, 0.5, 1)
        font.pixelSize: 15/strap
        textFormat: Text.RichText
        visible: showDbError
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 30/strap
    }

    states:[
        State {
            name: "showFinder"
            when: showFinder
        },
        State {
            name: "showFinderFast"
            when: showFinderFast
        },
        State {
            name: "hideFinde"
        },
        State {
            name: "showFilter"
        },
        State {
            name: "hideFilter"
        },
        State {
            name: "hideFilter_hideFinder"
        }
    ]
    transitions:[
        Transition {
            to: "showFinder"
            SequentialAnimation {
                NumberAnimation { target: root; easing.amplitude: 1.65; properties: "girlOpacity"; to: 1.0; duration: 800; easing.type: Easing.OutInElastic }
                ParallelAnimation {
                    NumberAnimation { target: root; properties: "blurOpacity"; to: 1; duration: 1700; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; properties: "blurRadius"; to: 40; duration: 1700; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 1.0; duration: 400; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "showFinderFast"
            SequentialAnimation {
                NumberAnimation { target: root; properties: "girlOpacity"; to: 0; duration: 200; easing.type: Easing.InOutQuart }
                ParallelAnimation {
                    NumberAnimation { target: root; properties: "girlOpacity"; to: 1.0; duration: 500; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; properties: "blurOpacity"; to: 1; duration: 700; easing.type: Easing.InOutQuart }
                    NumberAnimation { target: root; properties: "blurRadius"; to: 40; duration: 700; easing.type: Easing.InOutQuart }
                }
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 1.0; duration: 200; easing.type: Easing.InOutQuart }
                PropertyAction { target: input; property: "focus"; value: false }
                PropertyAction { target: input; property: "focus"; value: true }
                PropertyAction { target: root; property: "showFinderFast"; value: false }
            }
        },
        Transition {
            to: "hideFinder"
            SequentialAnimation {
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 0; duration: 300; easing.type: Easing.InOutQuart }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.65; properties: "blurOpacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                    NumberAnimation { target: root; easing.amplitude: 1.65; properties: "girlOpacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                }
                PropertyAction { target: root; property: "transition"; value: true }
                PropertyAction { target: root; property: "showWaitSpinner"; value: true }
            }
        },
        Transition {
            to: "showFilter"
            SequentialAnimation {
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 0; duration: 50; easing.type: Easing.InOutQuart }
                NumberAnimation { target: filtersLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: -screen.width; duration: 1100; easing.type: Easing.OutQuart }
            }
        },
        Transition {
            to: "hideFilter"
            SequentialAnimation {
                NumberAnimation { target: filtersLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: 0; duration: 700; easing.type: Easing.OutQuart }
                NumberAnimation { target: buttonsFindColumn; properties: "opacity"; to: 1; duration: 50; easing.type: Easing.InOutQuart }
            }
        },
        Transition {
            to: "hideFilter_hideFinder"
            SequentialAnimation {
                NumberAnimation { target: filtersLayer; easing.amplitude: 1.7; properties: "anchors.leftMargin"; to: 0; duration: 1100; easing.type: Easing.OutQuart }
                ParallelAnimation {
                    NumberAnimation { target: root; easing.amplitude: 1.65; properties: "blurOpacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                    NumberAnimation { target: root; easing.amplitude: 1.65; properties: "girlOpacity"; to: 0; duration: 800; easing.type: Easing.OutInElastic }
                }
                PropertyAction { target: root; property: "transition"; value: true }
                PropertyAction { target: root; property: "showWaitSpinner"; value: true }
            }
        }
    ]
}
// Espacios hechos.
