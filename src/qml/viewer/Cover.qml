import QtQuick 2.4

Item {
    id: cover

    Flipable {
        id: flipable
        width: front.width
        height: front.height
        front:
        Image {
            id: front
            width: screen.width/4.58
            height: screen.height/1.8
            sourceSize.width: screen.width/4.58
            sourceSize.height: screen.height/1.8
            source: hostCover+urlFrontCover
            onStatusChanged: {
                if (front.status === Image.Ready) {
                    viewerHandler.coverStatus.push(0)
                    check_cover_status();
                } else if (front.status === Image.Error) {
                    front.source = "qrc:/img-cover-err"
                    viewerHandler.coverStatus.push(1)
                    check_cover_status();
                }
            }
        }
        back:
        Image {
            id: back
            width: front.width
            height: front.height
            sourceSize.width: screen.width/4.58
            sourceSize.height: screen.height/1.8
            source: {
                if (urlBackCover !== "")
                    hostCover+urlBackCover
                else
                    front.source
            }
            onStatusChanged: {
                if (back.status === Image.Error)
                    back.source = front.source
            }
        }
        anchors.centerIn: parent
        property bool flipped
        transform: Rotation {
            id: rotation
            origin.x: flipable.width/2
            origin.y: flipable.height/2
            axis.x: 0
            axis.y: 1
            axis.z: 0
        }
        MouseArea {
            anchors.fill: parent
            onClicked: { flipable.flipped = !flipable.flipped }
        }
        states: State {
            name: "back"
            when: flipable.flipped
            PropertyChanges { target: rotation; angle: 180 }
        }
        transitions: Transition {
            ParallelAnimation {
                NumberAnimation { target: rotation; properties: "angle"; duration: 600 }
                SequentialAnimation {
                    NumberAnimation { target: flipable; property: "scale"; to: 0.75; duration: 300 }
                    NumberAnimation { target: flipable; property: "scale"; to: 1.0; duration: 300 }
                }
            }
        }
    }

    function check_cover_status() {
        if (viewerHandler.coverStatus.length === root.block_films)
            view.state = "show"
    }
}
// Tabs hechos.
