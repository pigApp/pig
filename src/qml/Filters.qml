import QtQuick 2.2

Rectangle {
    id: filters
    x: root.xAnimation
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
                            "qrc:/resources/images/category/"+categoryList[index+1]+".jpg"
                        else
                            "qrc:/resources/images/category/"+categoryList[index+1]+"_NOT_AVAILABLE.jpg"
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
                            "qrc:/resources/images/pornstars/"+pornstarList[index+1]+".jpg"
                        else
                            "qrc:/resources/images/pornstars/"+pornstarList[index+1]+"_NOT_AVAILABLE.jpg"
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

    onFocusChanged: { if (!filters.focus) filters.forceActiveFocus() }

    Component.onCompleted: { filters.forceActiveFocus() }
}
// Espacios hechos.
