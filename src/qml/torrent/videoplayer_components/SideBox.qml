import QtQuick 2.3
import QtMultimedia 5.0

Rectangle {
    id: sidebox
    color: Qt.rgba(0, 0, 0, 0.5)

    Column {
        id: timeColumn
        spacing: -15//
        anchors.centerIn: parent
        Text {
            id: currentLabel
            text: formatTime(player.position)
            color: "white"
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: totalLabel
            text: formatTime(player.duration)
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
    }
    Image {
        id: playerStatusIcon
        width: screen.width/60
        height: screen.height/33.75
        sourceSize.width: screen.width/60
        sourceSize.height: screen.height/33.75
        source: {
            if (videoPlayerHandler.standby) {
                "qrc:/img-standby"
            } else {
                if (player.playbackState === MediaPlayer.PlayingState )
                    "qrc:/img-playing"
                else if (player.playbackState === MediaPlayer.PausedState)
                    "qrc:/img-paused"
                else if (player.playbackState === MediaPlayer.StoppedState)
                    "qrc:/img-standby"
            }
        }
        opacity: 1
        anchors.top: timeColumn.bottom
        anchors.topMargin: 10//
        anchors.left: timeColumn.left
    }
    Row {
        id: volumeRow
        spacing: 10
        anchors.top: playerStatusIcon.bottom
        anchors.topMargin: 10//
        anchors.left: timeColumn.left
        Image {
            id: volumeIcon
            width: screen.width/60
            height: screen.height/33.75
            sourceSize.width: screen.width/60
            sourceSize.height: screen.height/33.75
            source: { if (player.volume !== 0.0 && !player.muted) "qrc:/img-volume_on"; else "qrc:/img-volume_off" }
            opacity: 1
        }
        Text {
            id: volumeLabel
            text: {
                if (!player.muted) {
                    if (player.volume === 0.0)
                        "0.0"
                    else if (player.volume === 1.0)
                        "1.0"
                    else
                        player.volume
                } else {
                    "0.0"
                }
            }
            color: {
                if (player.muted || player.volume === 0.0)
                    Qt.rgba(0.1, 0.1, 0.1, 1)
                else
                    "white"
            }
            font.family: globalFont.name
            font.pixelSize: screen.height/23
            anchors.verticalCenter: volumeIcon.verticalCenter
            anchors.verticalCenterOffset: -3.3
        }
    }
    Column {
        id: statusColumn
        spacing: -15//
        anchors.top: volumeRow.bottom
        anchors.topMargin: -20//
        anchors.left: timeColumn.left
        Text {
            id: bitRateLabel
            text: { if (root.bitRate !== "") root.bitRate }
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: globalFont.name
            font.pixelSize: screen.height/23
        }
        Text {
            id: peersLabel
            text: { if (root.peers !== "") root.peers }
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: globalFont.name
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
}
