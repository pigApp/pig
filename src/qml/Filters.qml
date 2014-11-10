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
                model: categoryList[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    labelText: categoryList[index+1]
                    indicatorLabelvisible: { if (nCategoryList[index] !== "0") true; else false }
                    numberLabelText: { if (nCategoryList[index] !== "0") nCategoryList[index]; else '' }
                    sourceImage: {
                        if (nCategoryList[index] !== "0")
                            "/resources/images/finder/category/"+categoryList[index+1]+".jpg"
                        else
                            "/resources/images/finder/category/NOT_AVAILABLE/"+categoryList[index+1]+"_NOT_AVAILABLE.jpg"
                    }
                    visible: { if (activeFilter === "CATEGORY" ) true; else false }
                    enabled: { if (activeFilter === "CATEGORY" ) true; else false }
                    onClicked: {
                        if (nCategoryList[index] !== "0")
                            filtersManager("categoryFilter", labelText)
                    }
                }
            }
            Repeater {
                id: pornstarRows
                model: pornstarList[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    labelText: pornstarList[index+1]
                    indicatorLabelvisible: { if (nPornstarList[index] !== "0") true; else false }
                    numberLabelText: { if (nPornstarList[index] !== "0") nPornstarList[index]; else '' }
                    sourceImage: {
                        if (nPornstarList[index] !== "0")
                            "/resources/images/finder/pornstars/"+pornstarList[index+1]+".jpg"
                        else
                            "/resources/images/finder/pornstars/NOT_AVAILABLE/"+pornstarList[index+1]+"_NOT_AVAILABLE.jpg"
                    }
                    visible: { if (activeFilter === "PORNSTAR") true; else false }
                    enabled: { if (activeFilter === "PORNSTAR") true; else false }
                    onClicked: {
                        if (nPornstarList[index] !== "0")
                            filtersManager("pornstarFilter", labelText)
                    }
                }
            }
        }
    }
}
// Tabs hechos.
