import QtQuick 2.2

Rectangle {
    id: filters
    color: "transparent"
    z: 1
    anchors.fill: parent

    Flickable {
        id: flickAreaFilters
        contentWidth: filtersGrid.width
        contentHeight: filtersGrid.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        anchors.fill: parent
        Grid {
            id: filtersGrid
            spacing: parent.width/384
            Repeater {
                id: categoryRows
                model: categoryList[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    labelText: categoryList[index+1]
                    nLabelText: { if (nCategoryList[index] !== '0') nCategoryList[index]; else '' }
                    pornStarsVisible: false
                    visible: { if (enableFilter === 'CATEGORY' ) true; else false }
                    enabled: { if (enableFilter === 'CATEGORY' ) true; else false }
                    onClicked: { 
                        if (nCategoryList[index] !== '0')
                            filtersManager('categoryFilter', labelText)
                    }
                }
            }
            Repeater {
                id: pornstarRows
                model: pornstarList[0]
                delegate: ButtonFilter {
                    width: filters.width/4
                    sourceImage: {
                        if (nPornstarList[index] !== '0')
                            "qrc:/images/pornstars/"+pornstarList[index+1]+".jpg"
                        else
                            "qrc:/images/available/pornstarNotAvailable.png"
                    }
                    labelText: pornstarList[index+1]
                    nLabelText: ''
                    pornStarsVisible: true
                    visible: { if (enableFilter === 'PORNSTAR') true; else false }
                    enabled: { if (enableFilter === 'PORNSTAR') true; else false }
                    onClicked: { 
                        if (nPornstarList[index] !== '0')
                        filtersManager('pornstarFilter', labelText)
                    }
                }
            }
        }
    }
}
// Espacios hechos.
