import QtQuick 2.9
import "../widgets"
import "../Vars.js" as Vars

Rectangle {
    width: 1280
    height: 1024
    color: "#c0c0c0"
    //"#2b7d99"
    Row{
        Podawanie{
            width: 640
            height: 480
            id:podawanie1
            label: "Podawanie 1"
        }
        Podawanie{
            width: 640
            height: 480
            id:podawanie2
            label: "Podawanie 2"
        }
    }
    ChartCanvas{
        width: 550
        height: 496
        anchors.left: parent.left
        y: 480
    }
    Wyciaganie{
        width: 730
        height: 496
        y: 480
        anchors.right: parent.right
    }
    StateButton{
        visible: false
        registerAddress: 2026
        latching: true
        label: "nip"
    }

    StateButton{
        visible: false
        anchors.centerIn: parent
        onClicked: {
            Qt.quit()
        }
    }
}
