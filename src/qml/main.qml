import QtQuick 2.1
import QtGraphicalEffects 1.0
import "core" 1.0 as Core

Item {
    id: root

    property bool requirePass
    property bool showAskPass
    property bool okPass
    property bool failPass
    property bool showCloseButton: true
    property string status
    property string statusInformation
    property real strap:1

    property bool showSpinner
    property bool showDecisionButton
    property bool restart
    property bool authorize
    property bool showFixDbButton

    property bool showFinder
    property bool showFinderFast
    property bool showDbError
    property bool showWaitSpinner
    property bool transition
    property string binaryVersion
    property string dataBaseVersion
    property string release
    property variant categoryList
    property variant pornstarList
    property bool noResult

    property bool showOutput
    property bool listUpdated
    property bool playerClosed
    property variant list
    property variant basicData
    property int totalFilms
    property int n

    property alias girlOpacity: girl.opacity
    property alias blurOpacity: blur.opacity
    property alias blurRadius: blur.radius

    signal passManager(string plain, bool init, bool write)
    signal updateAccept()
    signal updateCancel()
    signal updateRestart()
    signal findDb(string inputText, string category, string pornstar, int offset, bool init)
    signal openPlayer(string videoID)
    signal fixDb()
    signal quit()

    FontLoader { id: pigFont; source: "qrc:/images/font/pig.ttf" }
    
    Rectangle {
        id: screen
        color: "white"
        anchors.fill: parent

        Image {
            id: girl
            source: "qrc:/images/girl.png"
            cache: false
            fillMode: Image.PreserveAspectCrop
            opacity: 0
        }
        GaussianBlur {
            id: blur
            source: girl
            radius: 0
            samples: 32
            cached: true
            opacity: 0
            anchors.fill: girl
        }

        Core.AskPassword {
            id: askPasswordCore
            visible: requirePass && showAskPass
            enabled: requirePass && showAskPass
            anchors.fill: parent
        }

        Core.Finder {
            id: finderCore
            opacity: 0
            enabled: false
            anchors.fill: parent
            states: [
                State {
                    name: "show"
                    when: showFinder
                }
            ]
            transitions: [
                Transition {
                    to: "show"
                    PropertyAction { target: finderCore; property: "opacity"; value: 1.0 }
                    PropertyAction { target: finderCore; property: "enabled"; value: true }
                }
            ]
        }
        // Probar de al apretar Esc (back) destruir outputcore.
        Core.Output {
            id: outputCore
            opacity: 0
            enabled: false
            anchors.fill: parent
            states: [
                State {
                    name: "show";
                    when: showOutput
                    PropertyChanges { target: finderCore; enabled: false }
                    PropertyChanges { target: outputCore; enabled: true }
                    StateChangeScript {
                        name:"listCreator"
                        script: outputCore.listCreator(n, list);
                    }
                },
                State {
                    name: "updateList";
                    when: listUpdated
                    StateChangeScript {
                        name:"listCreator"
                        script: outputCore.listCreator();
                    }
                },
                State {
                    name: "back"
                    PropertyChanges { target: outputCore; enabled: false }
                    PropertyChanges { target: finderCore; opacity: 1.0 }
                    PropertyChanges { target: finderCore; enabled: true }
                }
            ]
            transitions: [
                Transition {
                    to: "show"
                    ScriptAction { scriptName:"listCreator" }
                    PropertyAction { target: root; property: "status"; value: "" }
                    PropertyAction { target: root; property: "statusInformation"; value: "" }
                },
                Transition {
                    to: "updateList"
                    ScriptAction { scriptName:"listCreator" }
                },
                Transition {
                    to: "back"
                    SequentialAnimation {
                        NumberAnimation { target: outputCore; property: "opacity"; to: 0; duration: 700; easing.type: Easing.InOutQuad }
                        PropertyAction { target: root; property: "showFinderFast"; value: true }
                   }
                }
            ]
        }

        Image {
            id: closeButton // Cambiar esto por un shortcut Esc+shift
            width: 14
            height: 12
            source: "qrc:/images/close.png"
            visible: showCloseButton
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: -1
            z:3
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: { quit() }
                onEntered: { closeButton.source = "qrc:/images/closeHover.png" }
                onHoveredChanged: { closeButton.source = "qrc:/images/close.png" }
            }
        }
    }

    onStatusChanged: {
        if(status === 'searching updates' || 'ERROR IN DATABASE') {
            if(root.width >= 1920)
                strap = 1
            else if(root.width > 1399 && root.width < 1920)
                strap = 1.1
            else if(root.width > 1023 && root.width < 1400)
                strap = 1.2
            else
                strap = 1
        }
    }

    Component.onCompleted: { finderCore.state = "show" }
}











/*   
     fALTA Px

     destruir webview al cerrarlo
     cambiar los bools de c++ a qml por signals
     usar tambien el valor de release en update, para saber la version del binario 

     La actualizacion en linux se puede remplazar el binario en ejecucion, no llamar a un programa aparte para actualizar, hacerlo desde id.cpp (solo en linux)
      
     falla url al reparar db
    .pro en windows
     Probar todo en windows
*/

/*
     FALTA LA PARTE DE VIDEO ESPERAR A QUE DEN SOPORTE A BLINK
     Dependencia gksu

    MANUAL
       |
       |__LINUX
       |     Copiar ultima db.sqlite ID_Linux/.ID/
       |
       |     Crear binario IDupdate x86
       |     |
       |     |_ Copiar binario IDupdate a ID_Linux/bin/x86/
       |
       |     Crear binario IDupdate x86_64
       |     |
       |     |_ Copiar binario IDupdate a ID_Linux/bin/x86_64/
       |
       |     Crear binario de ID x86
       |     |
       |     |_debe enlazar a /usr/lib/IDlib/
       |     |_debe tener una URL propia para descargas ID de x86
       |     |_copiar a ID_Linux/bin/x86/
       |
       |     Crear binario de ID x86_64
       |     |
       |     |_debe enlazar a /usr/lib64/IDlib64/
       |     |_debe tener una URL propia para descargas ID de x86_64
       |     |_copiar a ID_Linux/bin/x86_64/
       |
       |__WINDOWS
             Crear binario install.exe 32 y 64
             Crear binarios IDupdate.exe 32 y 64
             Crear binarios de ID.exe 32 y 64
             |_32
                 |_deben enlazar a IDlib
                 |_deben tener una URL propia para descargas ID.exe de 32
                 |_copiar a bin/
             |_64
                 |_deben enlazar a IDlib64
                 |_deben tener una URL propia para descargas ID.exe de 64
                 |_copiar a bin/

    PRO
      |
      |__LINUX+WINDOWS
            |
            |_ver si bin/ IDupdate y IDupdate.exe funcionan en 32
*/
