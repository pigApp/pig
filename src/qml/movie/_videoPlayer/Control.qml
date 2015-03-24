import QtQuick 2.4

Item {
    id: control

    property real mb_downloaded: root.mb_downloaded

    onMb_downloadedChanged: {

        console.log("PLAYER: "+((player.position/player.duration)+0.001))
        console.log("DOWN: "+(mb_downloaded/root.n_mb))

        if (!videoPlayer.sandbox) {
            if (!videoPlayer.stanby && (player.position/player.duration)+0.001
                >= (mb_downloaded/root.n_mb)) {
                videoPlayer.standby = true
                console.log("//// STANDBY")//
            } else if (videoPlayer.stanby && (player.position/player.duration)+0.001
                < (mb_downloaded/root.n_mb)) {
                videoPlayer.standby = false
                console.log("//// STANDBY-FALSE")//
            }
        }
    }
}

