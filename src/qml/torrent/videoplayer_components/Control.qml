import QtQuick 2.3

Item {
    id: control

    property real downloaded: root.downloaded_kb

    onDownloadedChanged: {
        if (!videoPlayerHandler.sandbox) {
            if (!videoPlayerHandler.stanby && (player.position/player.duration)+0.001 >= (downloaded/root.total_kb)) {
                videoPlayerHandler.standby = true
                console.log("//// STANDBY")
            } else if (videoPlayerHandler.stanby && (player.position/player.duration)+0.001 < (downloaded/root.total_kb)) {
                videoPlayerHandler.standby = false
                console.log("//// STANDBY-FALSE")
            }
        }
    }
}

