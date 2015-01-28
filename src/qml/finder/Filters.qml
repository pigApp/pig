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
                        root.categories[0]
                    else
                        root.pornstars[0]
                }
                delegate:
                FilterBox {
                    id: filterBox
                    width: filters.width/4
                    label: {
                        if (onFilterCategory)
                            root.categories[index+1]
                        else
                            root.pornstars[index+1]
                    }
                    label_n: {
                        if (onFilterCategory)
                            root.n_categories[index]
                        else
                            root.n_pornstars[index]
                    }
                    source: {
                        if (onFilterCategory)
                            "qrc:/img-cat-"+categories[index+1]
                        else
                            "qrc:/img-star-"+pornstars[index+1]
                    }
                    onClicked: {
                        filters.n = label_n
                        if (onFilterCategory)
                            set_filter(label)
                        else
                            set_filter(label)
                    }
                }
            }
        }
    }

    Text {
        id: labelCheckFilters
        text: "CHECK QUALITY FILTERS"
        color: "black"
        font.family: fontGlobal.name
        font.bold: true
        font.pixelSize: screen.height/54
        visible: false
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
    Timer {
        id: delayCheckFilters
        running: false
        repeat: false
        interval: 5000
        onTriggered: { labelCheckFilters.visible = false }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            finderHandler.state = "hide_filter"
            event.accepted = true
        } else if ((event.key === Qt.Key_Q) && (event.modifiers & Qt.ControlModifier)) {
            root.sig_qml_quit()
            event.accepted = true
        }
    }

    onFocusChanged: { if (!filters.focus) filters.forceActiveFocus() }

    Connections {
        target: cppSignals
        onSig_ret_db: {
            if ((n_films === 0) && (filters.n !== "000")) {
                labelCheckFilters.visible = true
                delayCheckFilters.start()
            }
        }
    }

    Component.onCompleted: filters.forceActiveFocus()
}
// Tabs hechos.
