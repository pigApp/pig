import QtQuick 2.4

Item {
    id: bars

    Rectangle {
       id: downloadBar
       width: { (parent.width*root.downloaded)/root.total }
       height: screen.height/540
       color: "gray"
       opacity: 0.5
       anchors.bottom: parent.bottom
    }
    Rectangle {
        id: timeBar
        width: { (parent.width*player.position)/player.duration }
        height: screen.height/540
        color: { if (videoPlayerHandler.standby) "yellow"; else "white" }
        anchors.bottom: parent.bottom
    }
}
