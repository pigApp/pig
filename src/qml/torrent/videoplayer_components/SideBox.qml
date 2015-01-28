import QtQuick 2.4
import QtMultimedia 5.4

Item {
    id: sidebox

    Column {
        spacing: parent.height/540
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            id: layerTime
            width: sidebox.width
            height: screen.height/10.8
            color: Qt.rgba(0, 0, 0, 0.5)
            Column {
                spacing: -screen.height/54
                anchors.left: parent.left
                anchors.leftMargin: screen.width/87.27
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id: labelCurrentTime
                    text: formatTime(player.position)
                    color: "white"
                    font.family: fontGlobal.name
                    font.pixelSize: screen.height/23
                }
                Text {
                    id: labelTotalTime
                    text: formatTime(player.duration)
                    color: Qt.rgba(0.1, 0.1, 0.1, 1)
                    font.family: fontGlobal.name
                    font.pixelSize: screen.height/23
                }
            }
        }
        Rectangle {
            id: layerPlayerState
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
                    id: iconPlayback
                    width: screen.width/58.18
                    height: screen.height/32.72
                    sourceSize.width: iconPlayback.width
                    sourceSize.height: iconPlayback.height
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
                    spacing: screen.width/192
                    Image {
                        id: iconVolume
                        width: screen.width/58.18
                        height: screen.height/32.72
                        sourceSize.width: iconVolume.width
                        sourceSize.height: iconVolume.height
                        source: { if ((player.volume !== 0.0) && !player.muted) "qrc:/img-volume"; else "qrc:/img-volume-off" }
                    }
                    Text {
                        id: labelVolume
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
                        font.family: fontGlobal.name
                        font.pixelSize: screen.height/23
                        anchors.verticalCenter: iconVolume.verticalCenter
                        anchors.verticalCenterOffset: -screen.height/360
                    }
                }
            }
        }
        Rectangle {
            id: layerStatus
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
                    spacing: screen.width/192
                    Image {
                        id: iconBitRate
                        width: screen.width/58.18
                        height: screen.height/32.72
                        sourceSize.width: iconBitRate.width
                        sourceSize.height: iconBitRate.height
                        source: "qrc:/img-download-dark"
                    }
                    Text {
                        id: labelBitRate
                        text: { if (root.bitRate !== "") root.bitRate }
                        color: Qt.rgba(0.1, 0.1, 0.1, 1)
                        font.family: fontGlobal.name
                        font.pixelSize: screen.height/23
                        anchors.verticalCenter: iconBitRate.verticalCenter
                        anchors.verticalCenterOffset: -screen.height/432
                    }
                    Text {
                        id: labelKb
                        text: { if (root.bitRate !== "") "Kb/s" }
                        color: Qt.rgba(0.1, 0.1, 0.1, 1)
                        font.family: fontGlobal.name
                        font.bold: true
                        font.pixelSize: screen.height/54
                        anchors.bottom: labelBitRate.bottom
                        anchors.bottomMargin: screen.height/154.28
                    }
                }
                Row {
                    spacing: screen.width/192
                    Image {
                        id: iconPeers
                        width: screen.width/58.18
                        height: screen.height/32.72
                        sourceSize.width: iconPeers.width
                        sourceSize.height: iconPeers.height
                        source: "qrc:/img-peers-dark"
                    }
                    Text {
                        id: labelPeers
                        text: { if (root.peers !== "") root.peers }
                        color: Qt.rgba(0.1, 0.1, 0.1, 1)
                        font.family: fontGlobal.name
                        font.pixelSize: screen.height/23
                        anchors.verticalCenter: iconPeers.verticalCenter
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
