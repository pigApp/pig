import QtQuick 2.4

Item {
    id: cover

    property bool cached
    property int id_cache: id_film

    Flipable {
        id: flipable
        width: front.width
        height: front.height
        front: Image {
            id: front
            width: screen.width/4.58
            height: screen.height/1.8
            sourceSize.width: width
            sourceSize.height: height
            source: { if (!cached) hostCover+urlCover; else "file://"+root.tmp+id_cache+"f.jpg" }
            onStatusChanged: {
                if (status === Image.Ready) {
                    viewerHandler.coverStatus.push(0)
                    check_cache_status();
                } else if (status === Image.Error) {
                    front.source = "qrc:/img-cover-err"
                    viewerHandler.coverStatus.push(1)
                    check_cache_status();
                }
            }
        }
        back: Image {
            id: back
            width: front.width
            height: front.height
            sourceSize.width: width
            sourceSize.height: height
            source: {
                if (urlCoverBack !== "")
                    if (!cached)
                        hostCover+urlCoverBack
                    else
                        "file://"+root.tmp+id_cache+"b.jpg"
                else
                    front.source
            }
            onStatusChanged: { if (status === Image.Error) source = front.source }
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

    function check_cache_status() {
        if (viewerHandler.coverStatus.length === viewerHandler.n_blockFilms)
            view.state = "show"
        if (!cached) {
            front.grabToImage(function(result) { result.saveToFile(root.tmp+id_cache+"f.jpg") })
            back.grabToImage(function(result) { if (result.saveToFile(root.tmp+id_cache+"b.jpg")) root.coverCache.push(id_cache) })
        }
    }

    Component.onCompleted: { if (root.coverCache.indexOf(id_cache) !== -1) cached = true }
}
// Tabs hechos.
