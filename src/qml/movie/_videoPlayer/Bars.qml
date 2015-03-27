import QtQuick 2.4

Item {
    id: bars

    property string status: root.status

    Rectangle {
        id: barTime
        width: { (parent.width*player.position)/player.duration }
        height: screen.height/540
        color: { if (videoPlayer.standby) "gold"; else "white" }
        anchors.bottom: parent.bottom
    }
    Rectangle {
       id: barDownload
       width: { (parent.width*root.mb_downloaded)/root.n_mb }
       height: screen.height/540
       color: "gray"
       opacity: 0.5
       anchors.bottom: parent.bottom
    }

    onStatusChanged: { if (status === "TORRENT ERROR") barDownload.color = "red" }
}
