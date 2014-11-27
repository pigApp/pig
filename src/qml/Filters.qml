import QtQuick 2.3

Item {
    id: filters
    x: screen.width
    width: screen.width
    height: screen.height

    Flickable {
        id: flickAreaFilters
        contentWidth: filtersGrid.width
        contentHeight: filtersGrid.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        anchors.margins: parent.width/640
        anchors.fill: parent
        Grid {
            id: filtersGrid
            spacing: 3
            Repeater {
                id: categoriesRepeater
                model: categories[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    labelText: categories[index+1]
                    numberLabelText: nCategories[index]
                    sourceImage: {
                        if (nCategories[index] !== "0") // TODO: Borrar
                            "/resources/images/finder/filters/categories/"+categories[index+1]+".jpg"
                        else
                            ""
                    }
                    visible: { if (activeFilter === "CATEGORY" ) true; else false }
                    enabled: { if (activeFilter === "CATEGORY" ) true; else false }
                    onClicked: filtersManager("categoryFilter", labelText)
                }
            }
            Repeater {
                id: pornstarsRepeater
                model: pornstars[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    labelText: pornstars[index+1]
                    numberLabelText: nPornstars[index]
                    sourceImage: {
                        if (nPornstars[index] !== "0") // TODO: Borrar
                            "/resources/images/finder/filters/pornstars/"+pornstars[index+1]+".jpg"
                        else
                            ""
                    }
                    visible: { if (activeFilter === "PORNSTAR") true; else false }
                    enabled: { if (activeFilter === "PORNSTAR") true; else false }
                    onClicked: filtersManager("pornstarFilter", labelText)
                }
            }
        }
    }
}
// Tabs hechos.
