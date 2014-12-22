import QtQuick 2.3

Item {
    id: filters
    x: screen.width
    width: screen.width
    height: screen.height

    Flickable {
        contentWidth: grid.width
        contentHeight: grid.height
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.margins: parent.width/640
        Grid {
            id: grid
            spacing: 3
            Repeater {
                model: {
                    if (onCategoryFilter)
                        categories[0]
                    else
                        pornstars[0]
                }
                delegate:
                FilterBox {
                    id: filterBox
                    width: filters.width/4
                    labelText: {
                        if (onCategoryFilter)
                            categories[index+1]
                        else
                            pornstars[index+1]
                    }
                    totalLabelText: {
                        if (onCategoryFilter)
                            totalCategories[index]
                        else
                            totalPornstars[index]
                    }
                    sourceImage: {
                        if (onCategoryFilter) {
                            if (totalCategories[index] !== "0")    // TODO: Borrar
                                "qrc:/img-cat-"+categories[index+1]
                            else
                                ""
                        } else {
                            if (totalPornstars[index] !== "0")    // TODO: Borrar
                                "qrc:/img-star-"+pornstars[index+1]
                            else
                                ""
                        }
                    }
                    onClicked: {
                        if (onCategoryFilter)
                            set_filter(labelText)
                        else
                            set_filter(labelText)
                    }
                }
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            finderHandler.state = "hide_filter"
            event.accepted = true
        } else if (event.key === Qt.Key_Q && (event.modifiers & Qt.ControlModifier)) {
            root.signal_qml_quit()
            event.accepted = true
        }
    }

    onFocusChanged: { if (!filters.focus) filters.forceActiveFocus() }

    Component.onCompleted: filters.forceActiveFocus()
}
// Tabs hechos.
