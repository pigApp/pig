import QtQuick 2.4

Item {
    id: control

    property real mb_downloaded: root.mb_downloaded

    onMb_downloadedChanged: {
        if (!videoPlayerHandler.sandbox) {
            if (!videoPlayerHandler.stanby && (player.position/player.duration)+0.001 >= (mb_downloaded/root.n_mb)) {
                videoPlayerHandler.standby = true
                console.log("//// STANDBY")//
            } else if (videoPlayerHandler.stanby && (player.position/player.duration)+0.001 < (mb_downloaded/root.n_mb)) {
                videoPlayerHandler.standby = false
                console.log("//// STANDBY-FALSE")//
            }
        }
    }
}

