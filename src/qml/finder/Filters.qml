import QtQuick 2.4

import "_filters/"

Item {
    id: filters
    x: root.xa-(screen.width/192)
    y: -screen.height/360
    width: screen.width
    height: screen.height

    property int n_box

    Flickable {
        contentWidth: grid.width
        contentHeight: grid.height
        flickableDirection: Flickable.VerticalFlick
        anchors.margins: parent.width/640
        anchors.fill: parent
        Grid {
            id: grid
            spacing: screen.height/360
            Repeater {
                model: n_box
                delegate: Box {
                    id: box
                    width: filters.width/4
                    color: { if (onCategory) "#FA6900"; else "#FD2790" }
                    label: {
                        if (onCategory)
                            if (index < root.categories.length) root.categories[index]; else ""
                        else
                            if (index < root.pornstars.length) root.pornstars[index]; else ""
                    }
                    label_n: {
                        if (onCategory)
                            if (index < root.categories.length) root.n_categories[index]; else ""
                        else
                            if (index < root.pornstars.length) root.n_pornstars[index]; else ""
                    }
                    source: {
                        if (onCategory)
                            if (index < root.categories.length) "qrc:/img-cat-"+categories[index]; else ""
                        else
                            if (index < root.pornstars.length) "qrc:/img-star-"+pornstars[index]; else ""
                    }
                }
            }
        }
    }

    Row {
        id: rowCheck
        spacing: screen.width/384
        visible: false
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Image {
            id: iconCheck
            width: screen.width/58.18
            height: screen.height/32.72
            sourceSize.width: width
            sourceSize.height: height
            source: "qrc:/img_warning"
            anchors.verticalCenter: labelCheck.verticalCenter
        }
        Text {
            id: labelCheck
            text: "CHECK QUALITY SELECTOR"
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/54
        }
    }
    Timer {
        id: delayCheck
        running: false
        repeat: false
        interval: 5000
        onTriggered: { rowCheck.visible = false }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            finder.state = "hide_filter"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    function set_filter(label) {
        if (onCategory)
            root.category = label.toUpperCase()
        else
            root.pornstar = label.toUpperCase()
        cpp.find("", root.category, root.pornstar, root.quality, root.full)
    }

    onFocusChanged: { if (!focus) forceActiveFocus() }

    Connections {
        target: cpp
        onSig_ret_db: {
            if (n_movies === 0) {
                root.category = ""
                root.pornstar = ""
                rowCheck.visible = true
                delayCheck.start()
            }
        }
    }

    Component.onCompleted: {
        if (onCategory)
            n_box = (Math.ceil(root.categories.length/16))*16
        else
            n_box = (Math.ceil(root.pornstars.length/16))*16
        forceActiveFocus()
    }
}
// Tabs hechos.
