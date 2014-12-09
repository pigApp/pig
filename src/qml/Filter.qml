import QtQuick 2.3

Item {
    id: filters
    x: screen.width
    width: screen.width
    height: screen.height

    Flickable {
        id: flickarea
        contentWidth: grid.width
        contentHeight: grid.height
        flickableDirection: Flickable.VerticalFlick
        anchors.margins: parent.width/640
        anchors.fill: parent
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
                    width: filters.width/4
                    labelText: {
                        if (onCategoryFilter)
                            categories[index+1]
                        else
                            pornstars[index+1]
                    }
                    numberLabelText: {
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
}
// Tabs hechos.
