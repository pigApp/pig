import QtQuick 2.4

Item {
    id: control
    enabled: false

    Timer {
        id: delayControl
        running: control.enabled
        repeat: true
        interval: 1000
        onTriggered: check()
    }

    function check() {
        console.log("PLAYER: "+((player.position/player.duration)+0.003))//
        console.log("DOWN: "+(mb_downloaded/root.n_mb))//

        if (((player.position/player.duration)+0.01) >= (root.mb_downloaded/root.n_mb)) {
            videoPlayer.standby = true
            console.log("//// STANDBY-TRUE")//
        } else if (((player.position/player.duration)+0.01) < (root.mb_downloaded/root.n_mb)) {
            videoPlayer.standby = false
            console.log("//// STANDBY-FALSE")//
        }
    }
}
