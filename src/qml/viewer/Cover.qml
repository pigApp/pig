import QtQuick 2.4

Item {
    id: cover

    property bool front_cached
    property bool back_cached
    property int id_cache: id_movie

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
            source: { if (!front_cached) hostCover+urlCoverFront; else "file://"+root.tmp+id_cache+"f.jpg" }
            onStatusChanged: {
                if (status === Image.Ready) {
                    viewer.coverStatus.push(0)
                    check_state("front");
                } else if (status === Image.Error) {
                    front.source = "qrc:/img-cover_err"
                    viewer.coverStatus.push(1)
                    check_state("front");
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
                    if (!back_cached)
                        hostCover+urlCoverBack
                    else
                        "file://"+root.tmp+id_cache+"b.jpg"
                else
                    front.source
            }
            onStatusChanged: {
                if (status === Image.Ready)
                    check_state("back");
                else if (status === Image.Error)
                    source = front.source
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

    function check_state(cover) {
        if (cover === "front") {
            if (viewer.coverStatus.length === viewer.n_blockMovies)
                view.state = "show"
            if (!front_cached)
                front.grabToImage(function(result) { if (result.saveToFile(root.tmp+id_cache+"f.jpg")) root.cache_cover_front.push(id_cache) })
        } else {
            if(!back_cached)
                back.grabToImage(function(result) { if (result.saveToFile(root.tmp+id_cache+"b.jpg")) root.cache_cover_back.push(id_cache) })
        }
    }

    Component.onCompleted: {
        //if (root.cache_cover_front.indexOf(id_cache) !== -1)
            front_cached = true
        //if (root.cache_cover_back.indexOf(id_cache) !== -1)
            back_cached = true
    }
}
// Tabs hechos.
