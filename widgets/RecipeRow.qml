import QtQuick 2.0

Rectangle {
    id: rect

    width: 600
    height: 80
    radius: 5
    state: "RELEASED"
    property string text:""
    border.color: "black"
    border.width: 1
    signal refresh()
    signal clicked()
    onRefresh:{
        text1.text=rect.text

    }

    Text{
        id:text1
        anchors.centerIn: parent
        font.pointSize: 16
        font.bold: true
    }

    states: [
        State {
            name: "PRESSED"
            PropertyChanges {
                target: rect
                color: "steelblue"
            }
            PropertyChanges {
                target: rect
                radius: 10
            }
        },
        State {
            name: "RELEASED"
            PropertyChanges {
                target: rect
                color: "white"
            }
            PropertyChanges {
                target: rect
                radius: 10
            }
        }
    ]

    transitions: [
        Transition {
            from: "PRESSED"
            to: "RELEASED"
            ColorAnimation {
                target: rect
                duration: 500
            }
        },
        Transition {
            from: "RELEASED"
            to: "PRESSED"
            ColorAnimation {
                target: rect
                duration: 500
            }
        }
    ]

    Connections {
        target: PLC
        onValuesUpdated: refresh()
    }

    MouseArea{
        anchors.fill:parent
        onClicked:parent.clicked()
    }
}
