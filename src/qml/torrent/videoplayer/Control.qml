import QtQuick 2.3

Item {
    id: control

    property real downloaded: root.downloaded_kb

    onDownloadedChanged: {
        if (!videoPlayer.sandbox)
            if (!videoPlayer.stanby && (player.position/player.duration)+0.001 >= (downloaded/root.total_kb)) {
                videoPlayer.standby = true
                console.log("//// STANDBY")
            } else if (videoPlayer.stanby && (player.position/player.duration)+0.001 < (downloaded/root.total_kb)) {
                videoPlayer.standby = false
                console.log("//// STANDBY-FALSE")
            }
        }
    }
}

