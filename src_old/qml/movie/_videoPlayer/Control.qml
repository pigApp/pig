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
        console.log("PLAYER_MS "+player.position)
        console.log("READY__MS "+((kb_writen*player.duration)/root.n_kb))
        console.log("N_MS "+player.duration)
        console.log("WRITEN_KB "+kb_writen)

        if ( player.position >= ((kb_writen*player.duration)/root.n_kb)) {
            videoPlayer.standby = true
            console.log("STANDBY-TRUE")//
        } else {
            videoPlayer.standby = false
            console.log("STANDBY-FALSE")//
        }
    }
}
