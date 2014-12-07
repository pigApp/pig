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
                FiltersButton {
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
                                "/resources/images/finder/filters/categories/"+categories[index+1]+".jpg"
                            else
                                ""
                        } else {
                            if (totalPornstars[index] !== "0")    // TODO: Borrar
                                "/resources/images/finder/filters/pornstars/"+pornstars[index+1]+".jpg"
                            else
                                ""
                        }
                    }
                    onClicked: {
                        if (onCategoryFilter)
                            filtersManager(labelText)
                        else
                            filtersManager(labelText)
                    }
                }
            }
        }
    }
}
// Tabs hechos.
