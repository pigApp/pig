import QtQuick 2.4
import "filters_components/"

Item {
    id: filters
    x: root.xa-(screen.width/192)
    y: -screen.height/360
    width: screen.width
    height: screen.height

    property string n

    Flickable {
        contentWidth: grid.width
        contentHeight: grid.height
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.margins: parent.width/640
        Grid {
            id: grid
            spacing: screen.height/360
            Repeater {
                model: {
                    if (onFilterCategory)
                        if (root.categories.length >= 32) root.categories.length; else 32
                    else
                        if (root.pornstars.length >= 32) root.pornstars.length; else 32
                }
                delegate:
                FilterBox {
                    id: filterBox
                    width: filters.width/4
                    color: { if (onFilterCategory) "#FA6900"; else "#FD2790" }
                    label: {
                        if (onFilterCategory)
                            if (index < root.categories.length) root.categories[index]; else ""
                        else
                            if (index < root.pornstars.length) root.pornstars[index]; else ""
                    }
                    label_n: {
                        if (onFilterCategory)
                            if (index < root.categories.length) root.n_categories[index]; else ""
                        else
                            if (index < root.pornstars.length) root.n_pornstars[index]; else ""
                    }
                    source: {
                        if (onFilterCategory)
                            if (index < root.categories.length) "qrc:/img-cat-"+categories[index]; else ""
                        else
                            if (index < root.pornstars.length) "qrc:/img-star-"+pornstars[index]; else ""
                    }
                    onClicked: {
                        filters.n = label_n
                        if (onFilterCategory)
                            set_filter(label)
                        else
                            set_filter(label)
                    } //TODO: ponerlos disables si estan vacios. No es 32, en la pantalla entran 16, tiene que ser multiplo de 16.
                }
            }
        }
    }

    Row {
        id: rowCheckFilter
        spacing: screen.width/384
        visible: false
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Image {
            id: iconCheckFilter
            width: screen.width/58.18
            height: screen.height/32.72
            sourceSize.width: iconCheckFilter.width
            sourceSize.height: iconCheckFilter.height
            source: "qrc:/img_warning"
            anchors.verticalCenter: labelCheckFilter.verticalCenter
        }
        Text {
            id: labelCheckFilter
            text: "CHECK QUALITY FILTER"
            color: "white"
            font.family: fontGlobal.name
            font.bold: true
            font.pixelSize: screen.height/54
        }
    }
    Timer {
        id: delayCheckFilter
        running: false
        repeat: false
        interval: 5000
        onTriggered: { rowCheckFilter.visible = false }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            finderHandler.state = "hide_filter"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            cpp.quit()
            event.accepted = true
        }
    }

    onFocusChanged: { if (!filters.focus) filters.forceActiveFocus() }

    Connections {
        target: cpp
        onSig_ret_db: {
            if ((n_films === 0) && (filters.n !== "000")) {
                rowCheckFilter.visible = true
                delayCheckFilter.start()
            }
        }
    }

    Component.onCompleted: filters.forceActiveFocus()
}
// Tabs hechos.
