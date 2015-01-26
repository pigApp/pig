import QtQuick 2.4
import QtMultimedia 5.4

Item {
    id: sidebox

    Column {
        id: globalColumn
        spacing: parent.height/540
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            id: timeLayer
            width: sidebox.width
            height: screen.height/10.8
            color: Qt.rgba(0, 0, 0, 0.5)
            Column {
                id: timeColumn
                spacing: -screen.height/54
                anchors.left: parent.left
                anchors.leftMargin: screen.width/87.27
                anchors.verticalCenter: parent.verticalCenter
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
        }
        Rectangle {
            id: playerStateLayer
            width: sidebox.width
            height: screen.height/10.8
            color: Qt.rgba(0, 0, 0, 0.5)
            Column {
                spacing: screen.height/154.28
                anchors.left: parent.left
                anchors.leftMargin: screen.width/80
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: screen.height/72
                Image {
                    id: playbackIcon
                    width: screen.width/58.18
                    height: screen.height/32.72
                    sourceSize.width: playbackIcon.width
                    sourceSize.height: playbackIcon.height
                    source: {
                        if (videoPlayerHandler.standby) {
                            "qrc:/img-standby"
                        } else {
                            if (player.playbackState === MediaPlayer.PlayingState)
                                "qrc:/img-playing"
                            else if (player.playbackState === MediaPlayer.PausedState)
                                "qrc:/img-paused"
                            else if (player.playbackState === MediaPlayer.StoppedState)
                                "qrc:/img-standby"
                        }
                    }
                }
                Row {
                    id: volumeRow
                    spacing: screen.width/192
                    Image {
                        id: volumeIcon
                        width: screen.width/58.18
                        height: screen.height/32.72
                        sourceSize.width: volumeIcon.width
                        sourceSize.height: volumeIcon.height
                        source: { if ((player.volume !== 0.0) && !player.muted) "qrc:/img-volume"; else "qrc:/img-volume-off" }
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
                        color: { if (player.muted || (player.volume === 0.0)) Qt.rgba(0.1, 0.1, 0.1, 1); else "white" }
                        font.family: globalFont.name
                        font.pixelSize: screen.height/23
                        anchors.verticalCenter: volumeIcon.verticalCenter
                        anchors.verticalCenterOffset: -screen.height/360
                    }
                }
            }
        }
        Rectangle {
            id: statusLayer
            width: sidebox.width
            height: screen.height/10.8
            color: Qt.rgba(0, 0, 0, 0.5)
            Column {
                spacing: -screen.height/49.09
                anchors.left: parent.left
                anchors.leftMargin: screen.width/80
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: screen.height/72
                Row {
                    id: bitRateRow
                    spacing: screen.width/192
                    Image {
                        id: bitRateIcon
                        width: screen.width/58.18
                        height: screen.height/32.72
                        sourceSize.width: bitRateIcon.width
                        sourceSize.height: bitRateIcon.height
                        source: "qrc:/img-download-dark"
                    }
                    Text {
                        id: bitRateLabel
                        text: { if (root.bitRate !== "") root.bitRate }
                        color: Qt.rgba(0.1, 0.1, 0.1, 1)
                        font.family: globalFont.name
                        font.pixelSize: screen.height/23
                        anchors.verticalCenter: bitRateIcon.verticalCenter
                        anchors.verticalCenterOffset: -screen.height/432
                    }
                    Text {
                        id: kbLabel
                        text: { if (root.bitRate !== "") "Kb/s" }
                        color: Qt.rgba(0.1, 0.1, 0.1, 1)
                        font.family: globalFont.name
                        font.bold: true
                        font.pixelSize: screen.height/54
                        anchors.bottom: bitRateLabel.bottom
                        anchors.bottomMargin: screen.height/154.28
                    }
                }
                Row {
                    id: peersRow
                    spacing: screen.width/192
                    Image {
                        id: peersIcon
                        width: screen.width/58.18
                        height: screen.height/32.72
                        sourceSize.width: peersIcon.width
                        sourceSize.height: peersIcon.height
                        source: "qrc:/img-peers-dark"
                    }
                    Text {
                        id: peersLabel
                        text: { if (root.peers !== "") root.peers }
                        color: Qt.rgba(0.1, 0.1, 0.1, 1)
                        font.family: globalFont.name
                        font.pixelSize: screen.height/23
                        anchors.verticalCenter: peersIcon.verticalCenter
                        anchors.verticalCenterOffset: -screen.height/432
                    }
                }
            }
        }
    }

    function formatTime(timeMs) {
        if (!timeMs || (timeMs <= 0)) return "00:00:00"
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
// Tabs hechos.
