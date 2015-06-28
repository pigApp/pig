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
       width: { (parent.width*((kb_writen*player.duration)/root.n_kb))/player.duration }
       height: screen.height/540
       color: "gray"
       opacity: 0.5
       anchors.bottom: parent.bottom
    }

    onStatusChanged: {
        if ((status === "TORRENT ERROR") && (root.bitRate === "0"))
            barDownload.color = "red"
    }
}
