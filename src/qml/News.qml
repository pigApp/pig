import QtQuick 2.0

Item {
    id: news

    Column {
        id: newColumn
        spacing: 7
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 100
        Text {
            id: binaryNews
            text: "NEWS FOR PIG "+binaryVersion+"."+release
            color: "black"
            font.family: pigLightFont.name
            font.bold: true
            font.pixelSize: 15
       }
       Text {
            id: binaryAdd
            text: "+ Support for custom torrent files.\n+ Support for custom torrent files."
            color: "green"
            font.family: pigLightFont.name
            font.bold: true
            font.pixelSize: 15
       }
       Text {
            id: binaryDelete
            text: "- Movie with no seeds."
            color: "red"
            font.family: pigLightFont.name
            font.bold: true
            font.pixelSize: 15
       }
       Text {
           id: dataBaseNews
           text: "\nNEWS FOR DATABASE "+databaseVersion
           color: "black"
           font.family: pigLightFont.name
           font.bold: true
           font.pixelSize: 15
      }
      Text {
           id: dataBaseAdd
           text: "+ Support for custom torrent files.\n+ Support for custom torrent files."
           color: "green"
           font.family: pigLightFont.name
           font.bold: true
           font.pixelSize: 15
      }
      Text {
           id: dataBaseDelete
           text: "- Movie with no seeds."
           color: "red"
           font.family: pigLightFont.name
           font.bold: true
           font.pixelSize: 15
      }
    }
}
