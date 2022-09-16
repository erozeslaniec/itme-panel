import QtQuick 2.0
import  "../icons"

Rectangle {
    width: 640
    height: 480
    color: "#c0c0c0"
    border.color: "#6e6c6c"
    property string label: "Podawanie"
    border.width: 2
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y:15
        color: "#343434"
        text: label
        font.pointSize: 28
    }

    Display{
        x: 73
        y: 101
        width: 240
        height: 120
        units: "m/min"
        label: "Prędkość podawania"

    }
    Display{
        x: 392
        y: 226
        width: 240
        height: 120
        units: "mm"
        label: "Zadana długość preformy"

    }
    Display{
        x: 392
        y: 100
        width: 240
        height: 120
        units: "mm"
        label: "Głębokość ładowania"

    }
    Display{
        x: 392
        y: 352
        width: 240
        height: 120
        units: "mm"
        label: "Długość użyteczna"

    }

    Grid{
        x: 8
        y: 227
        rows: 2
        columns: 3
        spacing: 5

        StateButton{
            iconURL: "../icons/doubleUp.png"
            onClicked: iconURL="../icons/doubleUpGrey.png"
        }
        StateButton{
            iconURL: "../icons/singleUp.png"
            onClicked: iconURL="../icons/singleUpGrey.png"

        }
        StateButton{
            iconURL: "../icons/startUp.png"
            onClicked: iconURL="../icons/startUpGrey.png"

        }
        StateButton{
            iconURL: "../icons/doubleDown.png"
            onClicked: iconURL="../icons/doubleDownGrey.png"

        }
        StateButton{
            iconURL: "../icons/singleDown.png"
            onClicked: iconURL="../icons/singleDownGrey.png"

        }
        StateButton{
            iconURL: "../icons/circleYellow.png"
            onClicked: iconURL="../icons/circleYellowGrey.png"

        }
    }

}
