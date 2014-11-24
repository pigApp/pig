import QtQuick 2.3

Rectangle {
    id: filters
    x: screen.width
    width: screen.width
    height: screen.height
    color: "black"

    Flickable {
        id: flickAreaFilters
        contentWidth: filtersGrid.width
        contentHeight: filtersGrid.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        anchors.fill: parent
        Grid {
            id: filtersGrid
            spacing: 0
            Repeater {
                id: categoryRows
                model: categories[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    labelText: categories[index+1]
                    indicatorLabelvisible: { if (nCategories[index] !== "0") true; else false }
                    numberLabelText: { if (nCategories[index] !== "0") nCategories[index]; else '' }
                    sourceImage: {
                        if (nCategories[index] !== "0")
                            "/resources/images/finder/categories/"+categories[index+1]+".jpg"
                        else
                            "/resources/images/finder/categories/NOT_AVAILABLE/"+categories[index+1]+"_NOT_AVAILABLE.jpg"
                    }
                    visible: { if (activeFilter === "CATEGORY" ) true; else false }
                    enabled: { if (activeFilter === "CATEGORY" ) true; else false }
                    onClicked: {
                        if (nCategories[index] !== "0")
                            filtersManager("categoryFilter", labelText)
                    }
                }
            }
            Repeater {
                id: pornstarRows
                model: pornstars[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    labelText: pornstars[index+1]
                    indicatorLabelvisible: { if (nPornstars[index] !== "0") true; else false }
                    numberLabelText: { if (nPornstars[index] !== "0") nPornstars[index]; else '' }
                    sourceImage: {
                        if (nPornstars[index] !== "0")
                            "/resources/images/finder/pornstars/"+pornstars[index+1]+".jpg"
                        else
                            "/resources/images/finder/pornstars/NOT_AVAILABLE/"+pornstars[index+1]+"_NOT_AVAILABLE.jpg"
                    }
                    visible: { if (activeFilter === "PORNSTAR") true; else false }
                    enabled: { if (activeFilter === "PORNSTAR") true; else false }
                    onClicked: {
                        if (nPornstars[index] !== "0")
                            filtersManager("pornstarFilter", labelText)
                    }
                }
            }
        }
    }
}
// Tabs hechos.
