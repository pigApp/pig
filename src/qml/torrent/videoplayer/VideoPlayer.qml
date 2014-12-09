import QtQuick 2.3
import QtMultimedia 5.0

Rectangle {
    id: videoPlayer
    //x: screen.width
    //width: screen.width
    //height: screen.height
    color: "black"
    anchors.fill: parent

    Video {
        id: player
        //source: "file:///home/lxfb/legoMovie.mp4"
        autoPlay: true
        fillMode: VideoOutput.PreserveAspectFit
        anchors.fill: parent
    }
    MouseArea {
        id: controlsPlayerMouseArea
        onClicked: {
            if (player.playbackState === MediaPlayer.PlayingState)
                player.pause()
            else if (player.playbackState === MediaPlayer.PausedState)
                player.play()
            else if (player.playbackState === MediaPlayer.StoppedState)
                player.play()
        }
        anchors.fill: parent
    }

    //
    Row {
        id: timeRow
        spacing: 20
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: currentTimeLabel
            text: formatTime(player.position)
            color: "white"
            font.family: globalFont.name
            font.bold: true
            font.pixelSize: screen.height/23
        }
        Text {
            id: totalTimeLabel
            text: formatTime(player.duration)
            color: "white"
            font.family: globalFont.name
            font.bold: true
            font.pixelSize: screen.height/23
        }
    }
    function formatTime(timeMs) {
        if (!timeMs || timeMs <= 0) return "00:00:00"
        var seconds = timeMs/1000;
        var minutes = Math.floor(seconds/60)
        var hours = Math.floor(minutes/60)
        seconds = Math.floor(seconds%60)
        minutes = Math.floor(minutes%60)
        hours = Math.floor(hours%60)
        if (seconds < 10) seconds = "0"+seconds;
        if (minutes < 10) minutes = "0"+minutes;
        if (hours < 10) hours = "0"+hours;
        return hours+":"+minutes+":"+seconds
    }

    Column {
        id: progressColumn
        spacing: 2
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        Rectangle {
            id: timeBar
            width: { videoPlayer.width*player.position/player.duration }
            height: 2
            color: "white"
        }
        Rectangle {
           id: torrentBar
           width: { (videoPlayer.width*player.position/player.duration)+10 }
           height: 2
           color: "gray"
           opacity: 0.5
        }
    }

    Component.onCompleted: { player.source = "file://"+root.sandbox }
}
// Tabs hechos.
