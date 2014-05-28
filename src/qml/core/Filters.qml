import QtQuick 2.1

Rectangle {
    id: filters
    color: "transparent"
    visible: showFilters
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
            spacing: 5
            Repeater {
                id: categoryRows
                model: categoryList[0]
                delegate: ButtonFilter {
                    width: widthFilters
                    labelText: categoryList[index+1]
                    nLabelText: { if (nCategoryList[index] != '0') nCategoryList[index]; else '' }
                    pornStarsVisible: false
                    visible: { if (showFilters && enabledFilters == 'CATEGORY' ) true; else false }
                    enabled: { if (showFilters && enabledFilters == 'CATEGORY' ) true; else false }
                    onClicked: { 
                        if (nCategoryList[index] != '0') 
                            filtersManager('categoryFind', labelText)
                    }
                }
            }
            Repeater {
                id: pornstarRows
                model: pornstarList[0]
                delegate: ButtonFilter {
                    width: widthFilters
                    sourceImage: { if (pornstarList[index+1] === "Asa Akira") "qrc:/images/Asa Akira.png"; else "qrc:/images/Holly Halston.png" }
                    labelText: pornstarList[index+1]
                    nLabelText: { if (nPornstarList[index] != '0') nPornstarList[index]; else '' }
                    pornStarsVisible: true
                    visible: { if (showFilters && enabledFilters == 'PORNSTAR') true; else false }
                    enabled: { if (showFilters && enabledFilters == 'PORNSTAR') true; else false }
                    onClicked: { 
                        if (nPornstarList[index] != '0') 
                        filtersManager('pornstarFind', labelText)
                    }
                }
            }
        }
    }
}
// Espacios hechos.
