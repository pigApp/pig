import QtQuick 2.3

Item {

    Rectangle {
       id: downloadBar
       width: { (parent.width*root.downloaded_kb)/root.total_kb }
       height: 2
       color: "gray"
       opacity: 0.5
       anchors.bottom: parent.bottom
    }
    Rectangle {
        id: timeBar
        width: { (parent.width*player.position)/player.duration }
        height: 2
        color: { if (videoPlayer.standby) "yellow"; else "white" }
        anchors.bottom: parent.bottom
    }
}
