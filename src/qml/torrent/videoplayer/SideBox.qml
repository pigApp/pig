import QtQuick 2.3
import QtMultimedia 5.0

Rectangle {
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
            if (videoPlayer.standby) {
                "qrc:/img-play"
            } else {
                if (player.playbackState === MediaPlayer.PlayingState )
                    "qrc:/img-play"
                else if (player.playbackState === MediaPlayer.PausedState)
                    "qrc:/img-download"
                else if (player.playbackState === MediaPlayer.StoppedState)
                    "qrc:/img-download"
            }
        }
        opacity: 0.4
        anchors.left: timeColumn.left
        anchors.top: timeColumn.bottom
        anchors.topMargin: 10//
    }
    Row {
        id: volumeRow
        spacing: 10
        anchors.left: timeColumn.left
        anchors.top: playerStatusIcon.bottom
        anchors.topMargin: 10//
        Image {
            id: volumeIcon
            width: screen.width/60
            height: screen.height/33.75
            sourceSize.width: screen.width/60
            sourceSize.height: screen.height/33.75
            source: {
                if (player.volume !== 0.0 && !player.muted)
                    "qrc:/img-play"
                else 
                    "qrc:/img-download"
            }
            opacity: 0.4
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
            color: Qt.rgba(0.1, 0.1, 0.1, 1)
            font.family: globalFont.name
            font.pixelSize: screen.height/23
            anchors.verticalCenter: volumeIcon.verticalCenter
            anchors.verticalCenterOffset: -2.3
        }
    }
    Column {
        id: downloadStatusColumn
        spacing: -15//
        anchors.left: timeColumn.left
        anchors.top: volumeRow.bottom
        anchors.topMargin: -20//
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
