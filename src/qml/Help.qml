import QtQuick 2.0

Text {
    id: pigLabel
    text: "PIG "+binaryVersion+"."+release+"<br>DB "+binaryVersion+"."+dataBaseVersion+"<br>PIG.NET"
    color: Qt.rgba(1, 1, 1, 1)
    font.family: pigFont.name
    font.pixelSize: 70
    anchors.centerIn: parent
}
